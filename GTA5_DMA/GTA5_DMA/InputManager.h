#pragma once
#include <Windows.h>
#include <string>
#include <chrono>
#include <memory.h>

class c_keys
{
private:
	uint64_t gafAsyncKeyStateExport = 0;
	uint8_t state_bitmap[64] { };
	uint8_t previous_state_bitmap[256 / 8] { };
	DWORD win_logon_pid = 0;
	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

	// Helper function to find signature in memory
	uint64_t FindSignature(uint64_t startAddress, uint64_t endAddress, const std::string& signature);

public:
	c_keys() = default;
	~c_keys() = default;

	bool InitKeyboard();
	void UpdateKeys();
	bool IsKeyDown(uint32_t virtual_key_code);
};

// Global instance for keyboard input
extern c_keys g_inputManager;