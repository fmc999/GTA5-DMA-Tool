#include "pch.h"
#include "InputManager.h"
#include "DMA.h"
#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include <chrono>

// Implementation based on DMALibrary

bool c_keys::InitKeyboard()
{
    // Get winlogon.exe PID
    this->win_logon_pid = 0;
    if (!VMMDLL_PidGetFromName(DMA::vmh, "winlogon.exe", &this->win_logon_pid)) {
        std::cerr << "Failed to get winlogon.exe PID" << std::endl;
        return false;
    }

    // Get module base for win32kbase.sys
    PVMMDLL_MAP_MODULEENTRY moduleInfo;
    if (!VMMDLL_Map_GetModuleFromNameW(DMA::vmh, this->win_logon_pid | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, L"win32kbase.sys", &moduleInfo, VMMDLL_MODULE_FLAG_NORMAL)) {
        std::cerr << "Failed to get win32kbase.sys module info" << std::endl;
        return false;
    }
    
    uint64_t win32kbaseBase = moduleInfo->vaBase;
    size_t win32kbaseSize = moduleInfo->cbImageSize;
    VMMDLL_MemFree(moduleInfo);

    // Get module base for win32k.sys or win32ksgd.sys
    uint64_t win32kBase = 0;
    size_t win32kSize = 0;
    
    if (VMMDLL_Map_GetModuleFromNameW(DMA::vmh, this->win_logon_pid | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, L"win32ksgd.sys", &moduleInfo, VMMDLL_MODULE_FLAG_NORMAL)) {
        win32kBase = moduleInfo->vaBase;
        win32kSize = moduleInfo->cbImageSize;
        VMMDLL_MemFree(moduleInfo);
    } else if (VMMDLL_Map_GetModuleFromNameW(DMA::vmh, this->win_logon_pid | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, L"win32k.sys", &moduleInfo, VMMDLL_MODULE_FLAG_NORMAL)) {
        win32kBase = moduleInfo->vaBase;
        win32kSize = moduleInfo->cbImageSize;
        VMMDLL_MemFree(moduleInfo);
    } else {
        std::cerr << "Failed to get win32k.sys/win32ksgd.sys module info" << std::endl;
        return false;
    }

    // Find g_session_global_slots signature
    uint64_t gSessionPtr = FindSignature(win32kBase, win32kBase + win32kSize, "48 8B 05 ? ? ? ? 48 8B 04 C8");
    if (!gSessionPtr) {
        // Try alternative signature for older versions
        gSessionPtr = FindSignature(win32kBase, win32kBase + win32kSize, "48 8B 05 ? ? ? ? FF C9");
        if (!gSessionPtr) {
            std::cerr << "Failed to find g_session_global_slots signature" << std::endl;
            return false;
        }
    }

    // Calculate g_session_global_slots address
    int relative = 0;
    VMMDLL_MemReadEx(DMA::vmh, this->win_logon_pid | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, gSessionPtr + 3, reinterpret_cast<BYTE*>(&relative), sizeof(int), nullptr, VMMDLL_FLAG_NOCACHE);
    uint64_t gSessionGlobalSlots = gSessionPtr + 7 + relative;

    // Find user session state
    uint64_t userSessionState = 0;
    for (int i = 0; i < 4; i++) {
        uint64_t slotAddr = 0;
        VMMDLL_MemReadEx(DMA::vmh, this->win_logon_pid | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, gSessionGlobalSlots + 8 * i, reinterpret_cast<BYTE*>(&slotAddr), sizeof(uint64_t), nullptr, VMMDLL_FLAG_NOCACHE);
        
        if (!slotAddr) continue;
        
        uint64_t sessionAddr = 0;
        VMMDLL_MemReadEx(DMA::vmh, this->win_logon_pid | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, slotAddr, reinterpret_cast<BYTE*>(&sessionAddr), sizeof(uint64_t), nullptr, VMMDLL_FLAG_NOCACHE);
        
        if (!sessionAddr) continue;
        
        VMMDLL_MemReadEx(DMA::vmh, this->win_logon_pid | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, sessionAddr, reinterpret_cast<BYTE*>(&userSessionState), sizeof(uint64_t), nullptr, VMMDLL_FLAG_NOCACHE);
        
        if (userSessionState > 0x7FFFFFFFFFFF) {
            break;
        }
    }

    if (userSessionState == 0) {
        std::cerr << "Failed to find valid user session state" << std::endl;
        return false;
    }

    // Find offset for gafAsyncKeyState
    uint64_t ptr = FindSignature(win32kbaseBase, win32kbaseBase + win32kbaseSize, "48 8D 90 ? ? ? ? E8 ? ? ? ? 0F 57 C0");
    if (!ptr) {
        std::cerr << "Failed to find offset for gafAsyncKeyState" << std::endl;
        return false;
    }

    // Calculate gafAsyncKeyStateExport address
    uint32_t sessionOffset = 0;
    VMMDLL_MemReadEx(DMA::vmh, this->win_logon_pid | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, ptr + 3, reinterpret_cast<BYTE*>(&sessionOffset), sizeof(uint32_t), nullptr, VMMDLL_FLAG_NOCACHE);
    this->gafAsyncKeyStateExport = userSessionState + sessionOffset;

    // Verify the address is valid (greater than kernel space threshold)
    return this->gafAsyncKeyStateExport > 0x7FFFFFFFFFFF;
}

// Helper function to find signature in memory
uint64_t c_keys::FindSignature(uint64_t startAddress, uint64_t endAddress, const std::string& signature)
{
    size_t sigLength = signature.length();
    if (sigLength == 0 || (endAddress - startAddress) < sigLength) {
        return 0;
    }

    // Convert signature string to bytes and mask
    std::vector<uint8_t> sigBytes;
    std::vector<bool> sigMask;
    
    for (size_t i = 0; i < sigLength; i += 3) {
        if (signature[i] == '?') {
            sigBytes.push_back(0x00);
            sigMask.push_back(false);
        } else {
            uint8_t byte = static_cast<uint8_t>(std::stoul(signature.substr(i, 2), nullptr, 16));
            sigBytes.push_back(byte);
            sigMask.push_back(true);
        }
    }

    // Read memory in chunks for efficiency
    const size_t chunkSize = 0x1000;
    std::vector<uint8_t> buffer(chunkSize + sigBytes.size());
    
    for (uint64_t addr = startAddress; addr <= endAddress - sigBytes.size(); addr += chunkSize) {
        size_t readSize = (addr + chunkSize <= endAddress) ? chunkSize : (endAddress - addr + 1);
        
        if (!VMMDLL_MemReadEx(DMA::vmh, this->win_logon_pid | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, addr, buffer.data(), static_cast<DWORD>(readSize), nullptr, VMMDLL_FLAG_NOCACHE)) {
            continue;
        }

        // Search for signature in the buffer
        for (size_t i = 0; i <= readSize - sigBytes.size(); i++) {
            bool match = true;
            for (size_t j = 0; j < sigBytes.size(); j++) {
                if (sigMask[j] && buffer[i + j] != sigBytes[j]) {
                    match = false;
                    break;
                }
            }
            
            if (match) {
                return addr + i;
            }
        }
    }

    return 0;
}

void c_keys::UpdateKeys()
{
    uint8_t previous_key_state_bitmap[64] = {0};
    memcpy(previous_key_state_bitmap, state_bitmap, 64);

    // Read the key state from kernel memory
    VMMDLL_MemReadEx(DMA::vmh, this->win_logon_pid | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, gafAsyncKeyStateExport, 
        reinterpret_cast<unsigned char*>(&state_bitmap), 64, nullptr, VMMDLL_FLAG_NOCACHE);
        
    for (int vk = 0; vk < 256; ++vk)
        if ((state_bitmap[(vk * 2 / 8)] & (1 << (vk % 4 * 2))) && 
            !(previous_key_state_bitmap[(vk * 2 / 8)] & (1 << (vk % 4 * 2))))
            previous_state_bitmap[vk / 8] |= (1 << (vk % 8));
}

bool c_keys::IsKeyDown(uint32_t virtual_key_code)
{
    if (gafAsyncKeyStateExport < 0x7FFFFFFFFFFF)
        return false;		
    
    // Update key states periodically
    if (std::chrono::system_clock::now() - start > std::chrono::milliseconds(100))
    {
        UpdateKeys();
        start = std::chrono::system_clock::now();
    }
    
    // Check if key is down
    return (state_bitmap[(virtual_key_code * 2 / 8)] & (1 << (virtual_key_code % 4 * 2))) != 0;
}

// Global instance
c_keys g_inputManager;