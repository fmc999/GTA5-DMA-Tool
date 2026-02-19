#include "pch.h"

#include "MyImGui.h"
#include "MyMenu.h"
#include "Features.h"
#include "Dev.h"
#include "ImGuiToolStyle.h"
#include "InputManager.h"

// 添加背景图片相关头文件
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wincodec.h>
#include <vector>
#include <string>

// 链接必要的库
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "windowscodecs.lib")

extern bool bAlive;

bool MyImGui::CreateDeviceD3D(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
		res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	if (res != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}

void  MyImGui::CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void  MyImGui::CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
	pBackBuffer->Release();
}

void  MyImGui::CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI  MyImGui::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    case WM_KEYDOWN:
        // Handle global key presses here if needed
        // For example, you could add special key handling here
        break;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

// 辅助函数：使用WIC加载PNG图片并创建D3D11纹理
bool MyImGui::LoadBackgroundTexture(const wchar_t* texturePath)
{
    // 初始化COM
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    // 创建WIC工厂
    IWICImagingFactory* wicFactory = nullptr;
    HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));
    if (FAILED(hr)) {
        CoUninitialize();
        return false;
    }

    // 创建解码器
    IWICBitmapDecoder* decoder = nullptr;
    hr = wicFactory->CreateDecoderFromFilename(
        texturePath,
        nullptr,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        &decoder
    );
    if (FAILED(hr)) {
        wicFactory->Release();
        CoUninitialize();
        return false;
    }

    // 获取帧
    IWICBitmapFrameDecode* frame = nullptr;
    hr = decoder->GetFrame(0, &frame);
    if (FAILED(hr)) {
        decoder->Release();
        wicFactory->Release();
        CoUninitialize();
        return false;
    }

    // 获取帧格式
    WICPixelFormatGUID pixelFormat;
    hr = frame->GetPixelFormat(&pixelFormat);
    if (FAILED(hr)) {
        frame->Release();
        decoder->Release();
        wicFactory->Release();
        CoUninitialize();
        return false;
    }

    // 转换为BGRA格式
    IWICFormatConverter* converter = nullptr;
    hr = wicFactory->CreateFormatConverter(&converter);
    if (FAILED(hr)) {
        frame->Release();
        decoder->Release();
        wicFactory->Release();
        CoUninitialize();
        return false;
    }

    hr = converter->Initialize(
        frame,
        GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone,
        nullptr,
        0.0f,
        WICBitmapPaletteTypeMedianCut
    );
    if (FAILED(hr)) {
        converter->Release();
        frame->Release();
        decoder->Release();
        wicFactory->Release();
        CoUninitialize();
        return false;
    }

    // 获取图片尺寸
    UINT width, height;
    hr = frame->GetSize(&width, &height);
    if (FAILED(hr)) {
        converter->Release();
        frame->Release();
        decoder->Release();
        wicFactory->Release();
        CoUninitialize();
        return false;
    }

    // 创建D3D11纹理描述
    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    // 读取像素数据
    UINT rowPitch = width * 4; // 4 bytes per pixel (BGRA)
    std::vector<BYTE> pixelData(width * height * 4);
    hr = converter->CopyPixels(nullptr, rowPitch, static_cast<UINT>(pixelData.size()), pixelData.data());
    if (FAILED(hr)) {
        converter->Release();
        frame->Release();
        decoder->Release();
        wicFactory->Release();
        CoUninitialize();
        return false;
    }

    // 创建纹理资源
    D3D11_SUBRESOURCE_DATA initData;
    ZeroMemory(&initData, sizeof(initData));
    initData.pSysMem = pixelData.data();
    initData.SysMemPitch = rowPitch;
    initData.SysMemSlicePitch = 0;

    ID3D11Texture2D* texture = nullptr;
    hr = g_pd3dDevice->CreateTexture2D(&textureDesc, &initData, &texture);
    if (FAILED(hr)) {
        converter->Release();
        frame->Release();
        decoder->Release();
        wicFactory->Release();
        CoUninitialize();
        return false;
    }

    // 创建着色器资源视图
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;

    hr = g_pd3dDevice->CreateShaderResourceView(texture, &srvDesc, &g_backgroundTexture);
    if (FAILED(hr)) {
        texture->Release();
        converter->Release();
        frame->Release();
        decoder->Release();
        wicFactory->Release();
        CoUninitialize();
        return false;
    }

    // 创建采样器状态
    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(samplerDesc));
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0.0f;
    samplerDesc.BorderColor[1] = 0.0f;
    samplerDesc.BorderColor[2] = 0.0f;
    samplerDesc.BorderColor[3] = 0.0f;
    samplerDesc.MinLOD = 0.0f;
    samplerDesc.MaxLOD = FLT_MAX;

    hr = g_pd3dDevice->CreateSamplerState(&samplerDesc, &g_samplerState);
    if (FAILED(hr)) {
        g_backgroundTexture->Release();
        g_backgroundTexture = nullptr;
        texture->Release();
        converter->Release();
        frame->Release();
        decoder->Release();
        wicFactory->Release();
        CoUninitialize();
        return false;
    }

    // 清理中间资源
    texture->Release();
    converter->Release();
    frame->Release();
    decoder->Release();
    wicFactory->Release();
    CoUninitialize();

    return true;
}

// 清理背景纹理
void MyImGui::CleanupBackgroundTexture()
{
    if (g_backgroundTexture) {
        g_backgroundTexture->Release();
        g_backgroundTexture = nullptr;
    }
    if (g_samplerState) {
        g_samplerState->Release();
        g_samplerState = nullptr;
    }
}

// 渲染背景图片
void MyImGui::RenderBackgroundImage()
{
    if (!g_backgroundTexture) {
        return;
    }

    // 只在非融合模式下渲染背景图片
    if (MyMenu::bFusionMode) {
        return;
    }

    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();

    // 计算图片位置和大小
    ImVec2 pos = ImVec2(0, 0);
    ImVec2 size = ImVec2(io.DisplaySize.x, io.DisplaySize.y);

    // 设置透明度（正常亮度）
    ImVec4 tintColor = ImVec4(1.0f, 1.0f, 1.0f, 0.8f); // 80% 透明度，使背景图片更清晰

    // 绘制图片 - 将ID3D11ShaderResourceView*转换为ImTextureID
    drawList->AddImage(
        reinterpret_cast<ImTextureID>(g_backgroundTexture),
        pos,
        size,
        ImVec2(0, 0),
        ImVec2(1, 1),
        ImColor(tintColor)
    );
}

bool MyImGui::Initialize()
{
	// Create application window
	ImGui_ImplWin32_EnableDpiAwareness();
	wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"GTA5_DMA Tool", nullptr };
	::RegisterClassExW(&wc);
	hwnd = ::CreateWindowW(wc.lpszClassName, L"GTA5_DMA - 多功能工具", WS_OVERLAPPEDWINDOW, 100, 100, 1400, 900, nullptr, nullptr, wc.hInstance, nullptr);

	// Initialize Direct3D
	if (!CreateDeviceD3D(hwnd))
	{
		CleanupDeviceD3D();
		::UnregisterClassW(wc.lpszClassName, wc.hInstance);
		return 1;
	}

	// Show the window
	::ShowWindow(hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(hwnd);

	// 加载背景图片
	LoadBackgroundTexture(L"BG.png");

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// Note: We're not enabling NavEnableKeyboard to allow global key listeners to work
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Load a Chinese font
	// Try to load Microsoft YaHei, SimHei, or other common Chinese fonts
	ImFont* font = nullptr;
	
	// Try various Chinese fonts in order of preference
	const char* fontPaths[] = {
		"C:/Windows/Fonts/msyh.ttc",      // Microsoft YaHei
		"C:/Windows/Fonts/msyhbd.ttc",    // Microsoft YaHei Bold
		"C:/Windows/Fonts/simhei.ttf",    // SimHei
		"C:/Windows/Fonts/simsun.ttc",    // SimSun
		"C:/Windows/Fonts/msjh.ttc",      // Microsoft JhengHei
	};
	
	for (const char* fontPath : fontPaths) {
		font = io.Fonts->AddFontFromFileTTF(fontPath, 18.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
		if (font) break;
	}
	
	// If no Chinese font is available, use the default font but with full Chinese glyph ranges
	if (!font) {
		io.Fonts->AddFontDefault();
		ImFontConfig config;
		config.MergeMode = true;
		// Use full Chinese range to ensure all characters are covered
		io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/msyh.ttc", 18.0f, &config, io.Fonts->GetGlyphRangesChineseFull());
	}

	// 设置ImGui样式
	ImGuiToolStyle::ApplyToolStyle();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Build the font atlas
	io.Fonts->Build();

	return 1;
}

bool MyImGui::Close()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// 清理背景图片资源
	CleanupBackgroundTexture();

	CleanupDeviceD3D();
	::DestroyWindow(hwnd);
	::UnregisterClassW(wc.lpszClassName, wc.hInstance);

	return 1;
}

bool MyImGui::OnFrame()
{
	// Poll and handle messages (inputs, window resize, etc.)
	// See the WndProc() function below for our to dispatch events to the Win32 backend.
	MSG msg;
	while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
			bAlive = false;
	}
	if (!bAlive)
		return 0;

	// Handle window being minimized or screen locked
	if (g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
	{
		::Sleep(10);
		return 1;
	}
	g_SwapChainOccluded = false;

	// Handle window resize (we don't resize directly in the WM_SIZE handler)
	if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
	{
		CleanupRenderTarget();
		g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
		g_ResizeWidth = g_ResizeHeight = 0;
		CreateRenderTarget();
	}

	// Handle fusion mode - enter fullscreen when enabled
	static bool bWasInFusionMode = false;
	if (MyMenu::bFusionMode && !bWasInFusionMode) {
		// Enter borderless fullscreen
		LONG style = GetWindowLong(hwnd, GWL_STYLE);
		SetWindowLong(hwnd, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(hwnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED);
		bWasInFusionMode = true;
	}
	else if (!MyMenu::bFusionMode && bWasInFusionMode) {
		// Exit fullscreen
		LONG style = GetWindowLong(hwnd, GWL_STYLE);
		SetWindowLong(hwnd, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
		SetWindowPos(hwnd, HWND_TOP, 100, 100, 1280, 800, SWP_FRAMECHANGED);
		bWasInFusionMode = false;
	}

	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// 渲染背景图片 - 最底层
	RenderBackgroundImage();

	// 首先渲染融合模式信息 - 背景图片之上
	if (MyMenu::bFusionMode) {
		MyMenu::RenderFusionMode();
	}

	// HOME键显示/隐藏菜单
	static bool bHomePressed = false;
	bool bHomeCurrentlyPressed = (GetAsyncKeyState(VK_HOME) & 0x8000) != 0; // 使用Windows API直接检测HOME键
	
	// 检测HOME键按下事件（上升沿）
	if (bHomeCurrentlyPressed && !bHomePressed) {
		bHomePressed = true;
		Dev::bShowAllMenus = !Dev::bShowAllMenus; // 切换菜单显示/隐藏
	}
	// 检测HOME键释放事件（下降沿）
	else if (!bHomeCurrentlyPressed && bHomePressed) {
		bHomePressed = false;
	}

	// Render all components
	if (Dev::bShowAllMenus) {
		MyMenu::Render();
		
		// WeaponInspector和VehicleEditor现在已集成到主菜单中，不再作为独立窗口渲染
		// 只渲染未集成到主菜单的功能
		if (Teleport::bEnable)
			Teleport::Render();
		
		// Dev功能在菜单显示时也显示
		Dev::Render();
	}
	
	// 在融合模式下显示活动功能 (即使隐藏了其他所有菜单)
	if (MyMenu::bFusionMode) {
		// 计算RGB彩色变换，确保颜色不会接近黑色
		float time = static_cast<float>(ImGui::GetTime());
		float r = sin(time * 1.0f) * 0.4f + 0.8f; // 范围 0.4-1.2 → 实际范围 0.4-1.0
		float g = sin(time * 1.3f) * 0.4f + 0.8f; // 范围 0.4-1.2 → 实际范围 0.4-1.0
		float b = sin(time * 1.7f) * 0.4f + 0.8f; // 范围 0.4-1.2 → 实际范围 0.4-1.0
		
		// 确保颜色不会太暗，所有通道最小值为0.6
		r = std::max(r, 0.6f);
		g = std::max(g, 0.6f);
		b = std::max(b, 0.6f);
		
		// 确保颜色不会超过1.0
		r = std::min(r, 1.0f);
		g = std::min(g, 1.0f);
		b = std::min(b, 1.0f);
		
		ImVec4 rainbowColor = ImVec4(r, g, b, 1.0f);
		
		// 直接渲染文字，没有窗口
		ImGui::SetNextWindowPos(ImVec2(0, 320), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(500, 200), ImGuiCond_Always);
		
		// 渲染文字在最底层 - 不使用ImGuiStyleVar_WindowBgAlpha，直接使用NoBackground标志
    ImGui::Begin("ActiveFeaturesOverlay", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoScrollbar);
		
		// 当前激活功能
		ImGui::SetWindowFontScale(1.2f);
		ImGui::TextColored(rainbowColor, "当前激活功能:");
		ImGui::SetWindowFontScale(1.0f);
		
		// 显示激活的功能，使用彩色文字
		if (WeaponInspector::bEnable) ImGui::TextColored(rainbowColor, "- 武器检查器");
		if (Teleport::bEnable) ImGui::TextColored(rainbowColor, "- 传送功能");
		if (RefreshHealth::bEnable) ImGui::TextColored(rainbowColor, "- 刷新生命值");
		if (NoWanted::bEnable) ImGui::TextColored(rainbowColor, "- 永不被通缉");
		if (GodMode::bPlayerGodMode.load()) ImGui::TextColored(rainbowColor, "- 玩家无敌");
		if (GodMode::bVehicleGodMode.load()) ImGui::TextColored(rainbowColor, "- 载具无敌");
		if (VehicleEditor::bEnable) ImGui::TextColored(rainbowColor, "- 载具编辑器");
		
		// 添加群号信息
		ImGui::SetWindowFontScale(1.2f);
		ImGui::TextColored(rainbowColor, "群: 1085350916");
		ImGui::SetWindowFontScale(1.0f);
		
		ImGui::End();
	}

	// ImGui::ShowDemoWindow();

	// Rendering
	ImGui::Render();
	
	// Set background color based on mode
	ImVec4 ClearColor;
	if (MyMenu::bFusionMode) {
		// Pure black background in fusion mode
		ClearColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	} else {
		// Transparent background to show the background image
		ClearColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	
	const float clear_color_with_alpha[4] = { ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.w };
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
	g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Present
	HRESULT hr = g_pSwapChain->Present(1, 0);   // Present with vsync
	//HRESULT hr = g_pSwapChain->Present(0, 0); // Present without vsync
	g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);

	return 1;
}