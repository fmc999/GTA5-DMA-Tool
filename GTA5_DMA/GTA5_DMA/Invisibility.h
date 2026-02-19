#pragma once
#include <atomic>

class Invisibility
{
public:
	static inline std::atomic<bool> bEnable = false;
	static inline std::atomic<bool> bInvisibility = false;
	
	// Non-atomic versions for UI
	static inline bool bEnableUI = false;
	static inline bool bInvisibilityUI = false;

	static void OnDMAFrame();
	static void UpdateInvisibility();
	static void SyncUI();
};