#include "pch.h"
#include <print>
#include <thread>

#include "MyImGui.h"

bool bAlive = true;

int main(int, char**)
{
	// Initialize ImGui
	if (!MyImGui::Initialize())
	{
		std::cerr << "Failed to initialize ImGui" << std::endl;
		return -1;
	}

	// Initialize DMA in a separate thread
	std::thread DMAThread([]() {
		if (DMA::Initialize())
		{
			DMA::DMAThreadEntry();
		}
	});

	// Main loop
	while (bAlive)
	{
		// Handle exit keys
		if (GetAsyncKeyState(VK_END) & 1)
			bAlive = false;

		// Render ImGui frame
		MyImGui::OnFrame();
	}

	// Clean up
	if (DMAThread.joinable())
		DMAThread.join();

	MyImGui::Close();

	return 0;
}
