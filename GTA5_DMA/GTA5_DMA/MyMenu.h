#pragma once

class MyMenu
{
public:
	static bool Render();
	
	// 菜单可见性变量
	static bool bMenuVisible;
	
	// 融合模式变量
	static bool bFusionMode;
	
	// Fullscreen mode variables
	static bool bFullscreenMode;
	
public:
	// 渲染融合模式窗口
	static void RenderFusionMode();
};