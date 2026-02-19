#include "pch.h"
#include "ArmorManager.h"
#include "DMA.h"
#include "Offsets.h"

// 静态成员变量定义
bool ArmorManager::bLockArmor = false;
float ArmorManager::currentArmor = 0.0f;

// 更新防弹衣值
bool ArmorManager::UpdateArmor()
{
    float value = 0.0f;
    if (ReadArmorFromMemory(value)) {
        currentArmor = value;
        return true;
    }
    return false;
}

// 设置防弹衣值
bool ArmorManager::SetArmor(float value)
{
    return WriteArmorToMemory(value);
}

// 每帧处理函数
bool ArmorManager::OnDMAFrame()
{
    // 处理防弹衣锁定逻辑
    if (bLockArmor) {
        SetArmor(200.0f);  // 设置为200
        currentArmor = 200.0f;  // 同步显示值为锁定值
    } else {
        // 只有在未锁定时才更新防弹衣值
        UpdateArmor();
    }
    
    return true;
}

// 从内存读取防弹衣值
bool ArmorManager::ReadArmorFromMemory(float& value)
{
    if (!DMA::vmh || !DMA::PID)
        return false;

    // 根据Cheat Engine配置读取防弹衣值:
    // "GTA5_Enhanced.exe"+043DBC98 -> 基址
    // 指针链: +8 -> +0x150C
    uintptr_t baseAddr = DMA::BaseAddress + Offsets::WorldPtr;
    if (!baseAddr) return false;
    
    // 读取基址值
    uintptr_t addr1 = 0;
    DWORD BytesRead = 0;
    VMMDLL_MemReadEx(DMA::vmh, DMA::PID, baseAddr, (BYTE*)&addr1, sizeof(uintptr_t), &BytesRead, VMMDLL_FLAG_NOCACHE);
    if (BytesRead != sizeof(uintptr_t) || !addr1) return false;
    
    // +8
    uintptr_t addr2 = 0;
    VMMDLL_MemReadEx(DMA::vmh, DMA::PID, addr1 + 0x8, (BYTE*)&addr2, sizeof(uintptr_t), &BytesRead, VMMDLL_FLAG_NOCACHE);
    if (BytesRead != sizeof(uintptr_t) || !addr2) return false;
    
    // 最终地址: +0x150C
    uintptr_t finalAddr = addr2 + 0x150C;
    
    // 读取防弹衣值
    VMMDLL_MemReadEx(DMA::vmh, DMA::PID, finalAddr, (BYTE*)&value, sizeof(float), &BytesRead, VMMDLL_FLAG_NOCACHE);
    if (BytesRead != sizeof(float)) return false;
    
    return true;
}

// 向内存写入防弹衣值
bool ArmorManager::WriteArmorToMemory(float value)
{
    if (!DMA::vmh || !DMA::PID)
        return false;

    // 根据Cheat Engine配置设置防弹衣值:
    // "GTA5_Enhanced.exe"+043DBC98 -> 基址
    // 指针链: +8 -> +0x150C
    uintptr_t baseAddr = DMA::BaseAddress + Offsets::WorldPtr;
    if (!baseAddr) return false;
    
    // 读取基址值
    uintptr_t addr1 = 0;
    DWORD BytesRead = 0;
    VMMDLL_MemReadEx(DMA::vmh, DMA::PID, baseAddr, (BYTE*)&addr1, sizeof(uintptr_t), &BytesRead, VMMDLL_FLAG_NOCACHE);
    if (BytesRead != sizeof(uintptr_t) || !addr1) return false;
    
    // +8
    uintptr_t addr2 = 0;
    VMMDLL_MemReadEx(DMA::vmh, DMA::PID, addr1 + 0x8, (BYTE*)&addr2, sizeof(uintptr_t), &BytesRead, VMMDLL_FLAG_NOCACHE);
    if (BytesRead != sizeof(uintptr_t) || !addr2) return false;
    
    // 最终地址: +0x150C
    uintptr_t finalAddr = addr2 + 0x150C;
    
    // 写入防弹衣值
    VMMDLL_MemWrite(DMA::vmh, DMA::PID, finalAddr, (BYTE*)&value, sizeof(float));
    
    return true;
}