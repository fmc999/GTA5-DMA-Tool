#include "pch.h"
#include "Invisibility.h"
#include "DMA.h"
#include "Offsets.h"

void Invisibility::OnDMAFrame()
{
	// Sync UI values to atomic values
	SyncUI();
	
	// Always update invisibility state based on UI setting
	UpdateInvisibility();
}

void Invisibility::UpdateInvisibility()
{
	// Invisibility effect: "GTA5_Enhanced.exe"+043DBC98 -> [8, 2C]
	// This is a multi-level pointer that needs to be resolved
	
	// First, get the base address from WorldPtr
	uintptr_t worldAddress = 0;
	DWORD bytesRead = 0;
	
	// Read the world pointer
	uintptr_t worldPtr = DMA::BaseAddress + Offsets::WorldPtr;
	VMMDLL_MemReadEx(DMA::vmh, DMA::PID, worldPtr, (BYTE*)&worldAddress, sizeof(uintptr_t), &bytesRead, VMMDLL_FLAG_NOCACHE);
	
	if (bytesRead != sizeof(uintptr_t) || !worldAddress) {
		return;
	}
	
	// Now read the player address (offset 0x8)
	uintptr_t playerAddress = 0;
	VMMDLL_MemReadEx(DMA::vmh, DMA::PID, worldAddress + 0x8, (BYTE*)&playerAddress, sizeof(uintptr_t), &bytesRead, VMMDLL_FLAG_NOCACHE);
	
	if (bytesRead != sizeof(uintptr_t) || !playerAddress) {
		return;
	}
	
	// Modify value: 47 = normal, 1 = invisible
	BYTE invisibilityValue = bInvisibility.load() ? 1 : 47;
	VMMDLL_MemWrite(DMA::vmh, DMA::PID, playerAddress + 0x2C, (BYTE*)&invisibilityValue, sizeof(BYTE));
}

void Invisibility::SyncUI()
{
	// Sync from UI to atomic values
	bInvisibility.store(bInvisibilityUI);
}