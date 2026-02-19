#pragma once
#include <cstdint>
#include <string>

// 游戏类型枚举
enum class GameType
{
	GTA5_Enhanced,
	GTA5,
	Unknown
};

// 全局游戏类型
extern GameType currentGameType;

namespace Offsets
{
	// 基础指针偏移量 - 来自GTA5_Enhanced最新修复传送报错版.CT
	static const uintptr_t WorldPtr_Enhanced = 0x44061E8;     // 48 8B 0D ? ? ? ? 49 8B 9E
	static const uintptr_t GlobalPtr_Enhanced = 0x47F2808;   // 48 8B 0D ? ? ? ? 0F 1F 44 00
	static const uintptr_t BlipPtr_Enhanced = 0x3EA6460;     // 4C 8D 3D ? ? ? ? 49 8B 34 C7
	static const uintptr_t TimeBasePtr_Enhanced = 0x47CDF70; // Time structure base address
	
	// GTA5.exe的偏移量
	static const uintptr_t WorldPtr_Original = 0x2603908;     // 48 8B 0D ? ? ? ? 49 8B 9E
	static const uintptr_t GlobalPtr_Original = 0x2FA8550;     // 48 8B 0D ? ? ? ? 0F 1F 44 00
	static const uintptr_t BlipPtr_Original = 0x206D600;       // 4C 8D 3D ? ? ? ? 49 8B 34 C7
	static const uintptr_t TimeBasePtr_Original = 0x47CDF70;    // Time structure base address
	
	// 当前使用的偏移量
	extern uintptr_t WorldPtr;
	extern uintptr_t GlobalPtr;
	extern uintptr_t BlipPtr;
	extern uintptr_t TimeBasePtr;
	
	// 根据包名设置偏移量和游戏类型
	inline void SetOffsetsByPackageName(const std::string& packageName)
	{
		if (packageName == "GTA5_Enhanced.exe")
		{
			WorldPtr = WorldPtr_Enhanced;
			GlobalPtr = GlobalPtr_Enhanced;
			BlipPtr = BlipPtr_Enhanced;
			TimeBasePtr = TimeBasePtr_Enhanced;
			currentGameType = GameType::GTA5_Enhanced;
		}
		else if (packageName == "GTA5.exe")
		{
			WorldPtr = WorldPtr_Original;
			GlobalPtr = GlobalPtr_Original;
			BlipPtr = BlipPtr_Original;
			TimeBasePtr = TimeBasePtr_Original;
			currentGameType = GameType::GTA5;
		}
		else
		{
			currentGameType = GameType::Unknown;
		}
	}
}