# GTA5 DMA Tool

一个基于 DMA（直接内存访问）技术的 GTA5 游戏修改工具，支持原版 GTA5 和 GTA5 Enhanced 版本。

## ⚠️ 免责声明

**本项目仅供学习和研究使用。** 使用本工具可能违反游戏的服务条款，可能导致账号封禁。开发者不对使用本工具造成的任何后果负责。

## 功能特性

### 核心功能
- ✅ **DMA 内存读写** - 使用 MemProcFS 进行安全的内存访问
- ✅ **双版本支持** - 同时支持 GTA5.exe 和 GTA5_Enhanced.exe
- ✅ **ImGui 图形界面** - 现代化的菜单界面
- ✅ **多线程架构** - DMA 读写与界面渲染分离

### 游戏功能
- 🛡️ **God Mode（无敌模式）** - 角色无敌
- ❤️ **Health Manager（生命管理）** - 生命和护甲控制
- 👮 **No Wanted（无通缉）** - 清除/禁用通缉等级
- 🚗 **Vehicle Editor（载具编辑器）** - 载具属性修改
- ⏰ **Time Control（时间控制）** - 游戏时间管理
- 🌪️ **Ragdoll（布娃娃）** -  ragdoll 效果控制
- ⚡ **Player Speed（玩家速度）** - 移动速度修改
- 👻 **Invisibility（隐身）** - 角色隐身
- 🚫 **No Collision（无碰撞）** - 禁用碰撞检测
- 🎯 **Player Chaser（玩家追踪）** - 追踪其他玩家
- 💸 **Heist Dividend（抢劫分红）** - 抢劫任务分红修改
- 🔫 **Weapon Inspector（武器检查器）** - 武器属性查看与修改
- 📍 **Teleport（传送）** - 快速传送到指定位置

## 环境要求

### 硬件
- 支持 DMA 的硬件设备（如 PCIe 采集卡、FPGA 等）
- 目标主机：Windows 系统，运行 GTA5
- 控制主机：Windows 系统

### 软件
- Visual Studio 2022（用于编译）
- MemProcFS 驱动和库
- .NET Framework 4.8（如需要）

### 依赖库
- **MemProcFS** - DMA 内存访问库
- **ImGui** - 图形用户界面库
- **DirectX 11** - 渲染后端

## 项目结构

```
GTA212312/
├── GTA5_DMA/              # 主项目
│   ├── GTA5_DMA/          # 源代码
│   │   ├── DMA.h/cpp      # DMA 核心类
│   │   ├── Offsets.h      # 游戏偏移量
│   │   ├── Features.h     # 功能模块列表
│   │   ├── main.cpp       # 程序入口
│   │   └── ...            # 各功能模块
│   ├── ImGui/             # ImGui 库
│   ├── MemProcFS/         # MemProcFS 库
│   └── x64/Release/       # 编译输出
└── SUBSTANCE-main/        # 辅助项目
```

## 编译说明

1. 使用 Visual Studio 2022 打开 `GTA5_DMA.sln`
2. 选择 Release / x64 配置
3. 确保 MemProcFS 相关文件（vmm.dll、leechcore.dll 等）在正确位置
4. 点击生成解决方案

## 使用说明

### 前置准备

1. **确保 DMA 硬件正常工作**
2. **在目标主机上启动 GTA5 游戏**
3. **确保 MemProcFS 驱动已加载**

### 运行步骤

1. 先启动 GTA5 游戏
2. 再运行 `GTA5_DMA.exe`
3. 程序会自动检测游戏版本（GTA5.exe 或 GTA5_Enhanced.exe）
4. 使用菜单启用/禁用各项功能

### 快捷键

- **END 键** - 退出程序
- **菜单控制** - 通过 ImGui 界面操作

## Offset 更新

游戏版本更新后，需要更新 `Offsets.h` 中的偏移量：

```cpp
namespace Offsets
{
    // GTA5 Enhanced 版本
    static const uintptr_t WorldPtr_Enhanced = 0x44061E8;
    static const uintptr_t GlobalPtr_Enhanced = 0x47F2808;
    static const uintptr_t BlipPtr_Enhanced = 0x3EA6460;
    
    // GTA5 原版
    static const uintptr_t WorldPtr_Original = 0x2603908;
    static const uintptr_t GlobalPtr_Original = 0x2FA8550;
    static const uintptr_t BlipPtr_Original = 0x206D600;
}
```

可以使用 Cheat Engine 或提供的 .CT 表格来查找新的偏移量。

## 技术细节

### DMA 架构

```
控制主机                    目标主机
┌─────────────┐            ┌─────────────┐
│ GTA5_DMA.exe│◄───DMA────►│   GTA5.exe  │
│             │  内存读写   │             │
└─────────────┘            └─────────────┘
```

### 核心类

- **DMA** - 内存读写核心类
- **MyImGui** - 界面管理类
- **各功能模块** - GodMode、NoWanted 等

## 常见问题

### Q: 程序无法找到游戏？
A: 确保先启动 GTA5 游戏，再运行本程序。

### Q: 功能不生效？
A: 检查偏移量是否与当前游戏版本匹配。

### Q: 如何更新偏移量？
A: 使用 Cheat Engine 扫描或参考社区提供的最新偏移量。

## 许可证

本项目仅供学习交流使用。

## 致谢

- MemProcFS 项目
- ImGui 项目
- GTA5 逆向工程社区

---

**版本**: 1.72  
**更新日期**: 2025.12.16
