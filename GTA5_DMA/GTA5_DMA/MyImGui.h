#pragma once

#include <d3d11.h>

class MyImGui
{
public:
	static inline ID3D11Device* g_pd3dDevice = nullptr;
	static inline ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
	static inline IDXGISwapChain* g_pSwapChain = nullptr;
	static inline bool g_SwapChainOccluded = false;
	static inline UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
	static inline ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
	static inline HWND hwnd = 0;
	static inline WNDCLASSEX wc = { 0 };

	// 背景图片相关
	static inline ID3D11ShaderResourceView* g_backgroundTexture = nullptr;
	static inline ID3D11SamplerState* g_samplerState = nullptr;

private:
	static bool CreateDeviceD3D(HWND hWnd);
	static void CleanupDeviceD3D();
	static void CreateRenderTarget();
	static void CleanupRenderTarget();
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// 背景图片相关方法
	static bool LoadBackgroundTexture(const wchar_t* texturePath);
	static void CleanupBackgroundTexture();
	static void RenderBackgroundImage();

public: 
	static bool Initialize();
	static bool Close();
	static bool OnFrame();
};