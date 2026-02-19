#include "pch.h"
#include "ImGuiToolStyle.h"

// 全局主题变量
static ThemeType currentTheme = ThemeType::SUBSTANCE_MODERN;

namespace ImGuiToolStyle {
    // 应用工具样式
    void ApplyToolStyle() {
        ApplyTheme(currentTheme);
    }
    
    // 应用指定主题
    void ApplyTheme(ThemeType theme) {
        currentTheme = theme;
        
        switch (theme) {
        case ThemeType::DEFAULT:
            SetupDefaultColors();
            break;
        case ThemeType::SUBSTANCE_MODERN:
            SetupSubstanceModernColors();
            break;
        case ThemeType::SUBSTANCE_DARK:
            SetupSubstanceDarkColors();
            break;
        case ThemeType::SUBSTANCE_LIGHT:
            SetupSubstanceLightColors();
            break;
        case ThemeType::SUBSTANCE_CYBERPUNK:
            SetupSubstanceCyberpunkColors();
            break;
        }
        
        SetupStyle();
    }
    
    // 获取当前主题
    ThemeType GetCurrentTheme() {
        return currentTheme;
    }
    
    // 默认主题颜色
    void SetupDefaultColors() {
        auto& colors = ImGui::GetStyle().Colors;
        
        // 现代化深色主题，采用蓝绿色+紫色渐变强调色，更具视觉冲击力
        colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.15f, 0.20f, 0.92f);       // 深灰蓝色窗口背景，半透明
        colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.24f, 0.95f);        // 子窗口背景，稍亮
        colors[ImGuiCol_PopupBg] = ImVec4(0.18f, 0.22f, 0.28f, 0.98f);       // 弹出窗口背景，更亮
        
        // Borders - 更细的边框，使用蓝绿色调
        colors[ImGuiCol_Border] = ImVec4(0.25f, 0.35f, 0.45f, 0.30f);         // 蓝绿色边框，半透明
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);   // 无阴影，更现代
        
        // Text - 更柔和的白色文本，带有轻微的蓝绿色调
        colors[ImGuiCol_Text] = ImVec4(0.95f, 0.98f, 1.00f, 1.00f);          // 亮白色文本
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.60f, 0.70f, 1.00f);   // 蓝灰色禁用文本
        
        // Headers - 现代化渐变蓝绿色强调色
        colors[ImGuiCol_Header] = ImVec4(0.20f, 0.60f, 0.80f, 0.90f);        // 蓝绿色标题
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.30f, 0.70f, 0.90f, 1.00f);  // 亮蓝绿色悬停效果
        colors[ImGuiCol_HeaderActive] = ImVec4(0.15f, 0.50f, 0.70f, 1.00f);  // 深蓝色激活效果
        
        // Buttons - 现代化渐变按钮样式
        colors[ImGuiCol_Button] = ImVec4(0.25f, 0.55f, 0.75f, 0.90f);        // 蓝绿色按钮
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.65f, 0.85f, 1.00f);  // 亮蓝绿色悬停
        colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.50f, 0.70f, 1.00f);  // 深蓝色激活
        
        // Frame - 现代化输入框样式，带有蓝绿色调
        colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.22f, 0.28f, 0.95f);       // 深灰蓝色输入框背景
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.28f, 0.35f, 0.98f); // 亮灰蓝色悬停背景
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.35f, 0.45f, 1.00f); // 蓝灰色激活背景
        
        // Tabs - 现代化标签页样式，蓝绿色主题
        colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.22f, 0.28f, 0.90f);          // 未激活标签，深灰蓝色
        colors[ImGuiCol_TabHovered] = ImVec4(0.25f, 0.35f, 0.45f, 1.00f);    // 蓝灰色悬停标签
        colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.60f, 0.80f, 1.00f);     // 蓝绿色激活标签
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.18f, 0.24f, 0.90f);  // 未聚焦标签
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.18f, 0.55f, 0.75f, 1.00f); // 未聚焦激活标签
        
        // Title bar - 现代化标题栏样式
        colors[ImGuiCol_TitleBg] = ImVec4(0.12f, 0.15f, 0.20f, 1.00f);      // 深灰蓝色标题栏背景
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.60f, 0.80f, 1.00f); // 蓝绿色激活标题栏
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.12f, 0.16f, 1.00f); // 折叠标题栏
        
        // Menu bar - 现代化菜单栏样式
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.24f, 1.00f);    // 深灰蓝色菜单栏背景
        
        // Scrollbar - 现代化滚动条样式
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.08f, 0.10f, 0.14f, 0.60f);   // 滚动条背景
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.25f, 0.35f, 0.45f, 0.70f);  // 蓝灰色滚动条滑块
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.35f, 0.45f, 0.55f, 0.90f); // 亮蓝灰色悬停滑块
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.45f, 0.55f, 0.65f, 1.00f);   // 激活滑块
        
        // Check box - 现代化勾选标记，蓝绿色调
        colors[ImGuiCol_CheckMark] = ImVec4(0.40f, 0.80f, 1.00f, 1.00f);     // 亮蓝色勾选标记
        
        // Slider - 现代化滑块样式，蓝绿色调
        colors[ImGuiCol_SliderGrab] = ImVec4(0.30f, 0.70f, 0.90f, 1.00f);    // 蓝绿色滑块
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.40f, 0.80f, 1.00f, 1.00f); // 亮蓝绿色激活滑块
        
        // Separator - 现代化分隔线样式，蓝绿色调
        colors[ImGuiCol_Separator] = ImVec4(0.25f, 0.35f, 0.45f, 0.40f);     // 蓝灰色分隔线
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.35f, 0.45f, 0.55f, 0.70f); // 亮蓝灰色悬停分隔线
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.45f, 0.55f, 0.65f, 1.00f);   // 激活分隔线
        
        // Resize grip - 现代化调整大小手柄，蓝绿色调
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.30f, 0.70f, 0.90f, 0.30f);    // 蓝绿色调整手柄
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.40f, 0.80f, 1.00f, 0.80f); // 亮蓝绿色悬停手柄
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.50f, 0.90f, 1.00f, 1.00f);   // 亮青色激活手柄
        
        // Plot - 现代化图表颜色，蓝绿色主题
        colors[ImGuiCol_PlotLines] = ImVec4(0.50f, 0.75f, 0.95f, 1.00f);     // 淡蓝绿色线条
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.30f, 0.70f, 0.90f, 1.00f); // 蓝绿色悬停线条
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.60f, 0.80f, 0.95f, 1.00f); // 蓝绿色直方图
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.40f, 0.90f, 1.00f, 1.00f); // 亮青色悬停直方图
        
        // Table - 现代化表格样式，蓝绿色主题
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.20f, 0.60f, 0.80f, 0.90f); // 蓝绿色表头
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.25f, 0.35f, 0.45f, 0.50f); // 蓝灰色强边框
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.20f, 0.28f, 0.38f, 0.30f);   // 淡蓝灰色弱边框
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);   // 透明行背景
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.05f, 0.08f, 0.12f, 0.80f); // 深蓝灰色交替行背景
        
        // Drag and drop - 现代化拖拽目标，亮蓝绿色
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.30f, 0.70f, 0.90f, 0.80f); // 蓝绿色拖拽目标
        
        // Navigation - 现代化导航高亮，蓝绿色
        colors[ImGuiCol_NavHighlight] = ImVec4(0.30f, 0.70f, 0.90f, 0.80f);  // 蓝绿色导航高亮
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.95f, 0.98f, 1.00f, 0.70f); // 亮白色窗口导航高亮
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);     // 黑色半透明背景
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.05f, 0.10f, 0.15f, 0.80f);     // 深蓝灰色半透明背景
    }
    
    // SUBSTANCE现代主题颜色
    void SetupSubstanceModernColors() {
        auto& colors = ImGui::GetStyle().Colors;
        
        // SUBSTANCE现代主题颜色 - 基于参考图片设计
        colors[ImGuiCol_WindowBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.60f);     // 灰色窗口背景，更透明（毛玻璃效果）
        colors[ImGuiCol_ChildBg] = ImVec4(0.25f, 0.25f, 0.25f, 0.65f);        // 子窗口背景，稍亮，更透明
        colors[ImGuiCol_PopupBg] = ImVec4(0.30f, 0.30f, 0.30f, 0.75f);       // 弹出窗口背景，更亮，更透明
        
        // Borders - 更细的边框，使用现代灰色调
        colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);         // 灰色边框
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);   // 无阴影，更现代
        
        // Text - 更柔和的白色文本
        colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);            // 亮白色文本
        colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);     // 灰色禁用文本
        
        // Headers - 现代化强调色
        colors[ImGuiCol_Header] = ImVec4(0.90f, 0.30f, 0.30f, 1.00f);          // 红色标题
        colors[ImGuiCol_HeaderHovered] = ImVec4(1.00f, 0.40f, 0.40f, 1.00f);   // 亮红色悬停效果
        colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 0.30f, 0.30f, 1.00f);    // 红色激活效果
        
        // Buttons - 现代化按钮样式
        colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 0.90f);          // 灰色按钮
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);   // 亮灰色悬停
        colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 0.30f, 0.30f, 1.00f);    // 红色激活
        
        // Frame - 现代化输入框样式
        colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.95f);         // 灰色输入框背景
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.25f, 0.98f);  // 亮灰色悬停背景
        colors[ImGuiCol_FrameBgActive] = ImVec4(1.00f, 0.30f, 0.30f, 1.00f);   // 红色激活背景
        
        // Tabs - 现代化标签页样式
        colors[ImGuiCol_Tab] = ImVec4(0.20f, 0.20f, 0.20f, 0.90f);            // 灰色未激活标签
        colors[ImGuiCol_TabHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);      // 亮灰色悬停标签
        colors[ImGuiCol_TabActive] = ImVec4(1.00f, 0.30f, 0.30f, 1.00f);       // 红色激活标签
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.15f, 0.15f, 0.90f);    // 未聚焦标签
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.80f, 0.20f, 0.20f, 1.00f); // 未聚焦激活标签
        
        // Title bar - 现代化标题栏样式
        colors[ImGuiCol_TitleBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);        // 深灰色标题栏背景
        colors[ImGuiCol_TitleBgActive] = ImVec4(1.00f, 0.30f, 0.30f, 1.00f); // 红色激活标题栏
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f); // 折叠标题栏
        
        // Menu bar - 现代化菜单栏样式
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);      // 稍亮的菜单栏背景
        
        // Scrollbar - 现代化滚动条样式
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.60f);     // 滚动条背景
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.30f, 0.30f, 0.30f, 0.70f);   // 灰色滚动条滑块
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.90f); // 亮灰色悬停滑块
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 0.30f, 0.30f, 1.00f); // 红色激活滑块
        
        // Check box - 现代化勾选标记
        colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.30f, 0.30f, 1.00f);       // 红色勾选标记
        
        // Slider - 现代化滑块样式
        colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 0.30f, 0.30f, 1.00f);       // 红色滑块
        colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.40f, 0.40f, 1.00f); // 亮红色激活滑块
        
        // Separator - 现代化分隔线样式
        colors[ImGuiCol_Separator] = ImVec4(0.25f, 0.25f, 0.25f, 0.40f);       // 灰色分隔线
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.30f, 0.30f, 0.30f, 0.70f); // 亮灰色悬停分隔线
        colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 0.30f, 0.30f, 1.00f);  // 红色激活分隔线
        
        // Resize grip - 现代化调整大小手柄
        colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 0.30f, 0.30f, 0.30f);       // 红色调整手柄
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 0.40f, 0.40f, 0.80f); // 亮红色悬停手柄
        colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.50f, 0.50f, 1.00f);  // 亮红色激活手柄
        
        // Plot - 现代化图表颜色
        colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.30f, 0.30f, 1.00f);         // 红色线条
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.40f, 0.40f, 1.00f);   // 亮红色悬停线条
        colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.30f, 0.30f, 1.00f);       // 红色直方图
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.40f, 0.40f, 1.00f); // 亮红色悬停直方图
        
        // Table - 现代化表格样式
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);     // 灰色表头
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.25f, 0.25f, 0.25f, 0.50f);  // 灰色强边框
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.20f, 0.20f, 0.20f, 0.30f);    // 淡灰色弱边框
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);          // 透明行背景
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.15f, 0.15f, 0.15f, 0.80f);       // 深色交替行背景
        
        // Drag and drop - 现代化拖拽目标
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 0.30f, 0.30f, 0.80f);   // 红色拖拽目标
        
        // Navigation - 现代化导航高亮
        colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.30f, 0.30f, 0.80f);       // 红色导航高亮
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.90f, 0.90f, 0.90f, 0.70f); // 亮白色窗口导航高亮
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);   // 黑色半透明背景
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.80f);     // 深灰色半透明背景
    }
    
    // SUBSTANCE深色主题颜色
    void SetupSubstanceDarkColors() {
        auto& colors = ImGui::GetStyle().Colors;
        
        // SUBSTANCE深色主题颜色
        colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.10f, 0.95f);       // 深黑色窗口背景
        colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.10f, 0.12f, 0.95f);        // 稍亮的子窗口背景
        colors[ImGuiCol_PopupBg] = ImVec4(0.12f, 0.12f, 0.14f, 0.98f);        // 弹出窗口背景
        
        // Borders - 更细的边框，使用深色调
        colors[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.25f, 0.30f);         // 深灰色边框
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);    // 无阴影，更现代
        
        // Text - 更柔和的白色文本
        colors[ImGuiCol_Text] = ImVec4(0.85f, 0.88f, 0.90f, 1.00f);            // 灰白色文本
        colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);     // 深灰色禁用文本
        
        // Headers - 现代化强调色
        colors[ImGuiCol_Header] = ImVec4(0.30f, 0.60f, 0.90f, 0.90f);           // 蓝色标题
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.40f, 0.70f, 1.00f, 1.00f);    // 亮蓝色悬停效果
        colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.50f, 0.80f, 1.00f);     // 深蓝色激活效果
        
        // Buttons - 现代化按钮样式
        colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.25f, 0.90f);           // 深灰色按钮
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.30f, 0.35f, 1.00f);     // 亮灰色悬停
        colors[ImGuiCol_ButtonActive] = ImVec4(0.30f, 0.60f, 0.90f, 1.00f);      // 蓝色激活
        
        // Frame - 现代化输入框样式
        colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.25f, 0.95f);           // 深灰色输入框背景
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.30f, 0.30f, 0.35f, 0.98f);    // 亮灰色悬停背景
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.60f, 0.90f, 1.00f);     // 蓝色激活背景
        
        // Tabs - 现代化标签页样式
        colors[ImGuiCol_Tab] = ImVec4(0.20f, 0.20f, 0.25f, 0.90f);               // 深灰色未激活标签
        colors[ImGuiCol_TabHovered] = ImVec4(0.30f, 0.30f, 0.35f, 1.00f);         // 亮灰色悬停标签
        colors[ImGuiCol_TabActive] = ImVec4(0.30f, 0.60f, 0.90f, 1.00f);          // 蓝色激活标签
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.15f, 0.20f, 0.90f);        // 未聚焦标签
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.25f, 0.55f, 0.85f, 1.00f);   // 未聚焦激活标签
        
        // Title bar - 现代化标题栏样式
        colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.10f, 1.00f);           // 深黑色标题栏背景
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.30f, 0.60f, 0.90f, 1.00f);      // 蓝色激活标题栏
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.05f, 0.05f, 0.07f, 1.00f);    // 折叠标题栏
        
        // Menu bar - 现代化菜单栏样式
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);          // 稍亮的菜单栏背景
        
        // Scrollbar - 现代化滚动条样式
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.07f, 0.60f);         // 滚动条背景
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.20f, 0.25f, 0.70f);       // 深灰色滚动条滑块
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.30f, 0.30f, 0.35f, 0.90f); // 亮灰色悬停滑块
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.30f, 0.60f, 0.90f, 1.00f);  // 蓝色激活滑块
        
        // Check box - 现代化勾选标记
        colors[ImGuiCol_CheckMark] = ImVec4(0.30f, 0.60f, 0.90f, 1.00f);           // 蓝色勾选标记
        
        // Slider - 现代化滑块样式
        colors[ImGuiCol_SliderGrab] = ImVec4(0.30f, 0.60f, 0.90f, 1.00f);           // 蓝色滑块
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.40f, 0.70f, 1.00f, 1.00f);      // 亮蓝色激活滑块
        
        // Separator - 现代化分隔线样式
        colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.20f, 0.25f, 0.40f);           // 深灰色分隔线
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.30f, 0.30f, 0.35f, 0.70f);     // 亮灰色悬停分隔线
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.30f, 0.60f, 0.90f, 1.00f);      // 蓝色激活分隔线
        
        // Resize grip - 现代化调整大小手柄
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.30f, 0.60f, 0.90f, 0.30f);           // 蓝色调整手柄
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.40f, 0.70f, 1.00f, 0.80f);    // 亮蓝色悬停手柄
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.50f, 0.80f, 1.00f, 1.00f);     // 亮青色激活手柄
        
        // Plot - 现代化图表颜色
        colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.70f, 1.00f, 1.00f);            // 蓝色线条
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.30f, 0.60f, 0.90f, 1.00f);     // 深蓝色悬停线条
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.50f, 0.80f, 1.00f, 1.00f);         // 亮蓝色直方图
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.30f, 0.60f, 0.90f, 1.00f);  // 深蓝色悬停直方图
        
        // Table - 现代化表格样式
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.30f, 0.60f, 0.90f, 0.90f);        // 蓝色表头
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.20f, 0.20f, 0.25f, 0.50f);     // 深灰色强边框
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.15f, 0.15f, 0.20f, 0.30f);      // 淡灰色弱边框
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);            // 透明行背景
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.05f, 0.05f, 0.07f, 0.80f);         // 深色交替行背景
        
        // Drag and drop - 现代化拖拽目标
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.30f, 0.60f, 0.90f, 0.80f);        // 蓝色拖拽目标
        
        // Navigation - 现代化导航高亮
        colors[ImGuiCol_NavHighlight] = ImVec4(0.30f, 0.60f, 0.90f, 0.80f);          // 蓝色导航高亮
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.85f, 0.88f, 0.90f, 0.70f);  // 灰白色窗口导航高亮
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.80f);      // 黑色半透明背景
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.03f, 0.03f, 0.05f, 0.90f);      // 深黑色半透明背景
    }
    
    // SUBSTANCE亮色主题颜色
    void SetupSubstanceLightColors() {
        auto& colors = ImGui::GetStyle().Colors;
        
        // SUBSTANCE亮色主题颜色
        colors[ImGuiCol_WindowBg] = ImVec4(0.95f, 0.95f, 0.95f, 0.95f);       // 浅灰色窗口背景
        colors[ImGuiCol_ChildBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.95f);        // 白色子窗口背景
        colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);        // 弹出窗口背景
        
        // Borders - 更细的边框，使用浅色调
        colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.30f);         // 浅灰色边框
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);    // 无阴影，更现代
        
        // Text - 深色文本
        colors[ImGuiCol_Text] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);            // 深灰色文本
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);     // 灰色禁用文本
        
        // Headers - 现代化强调色
        colors[ImGuiCol_Header] = ImVec4(0.20f, 0.50f, 0.80f, 0.90f);           // 蓝色标题
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.30f, 0.60f, 0.90f, 1.00f);    // 亮蓝色悬停效果
        colors[ImGuiCol_HeaderActive] = ImVec4(0.15f, 0.40f, 0.70f, 1.00f);     // 深蓝色激活效果
        
        // Buttons - 现代化按钮样式
        colors[ImGuiCol_Button] = ImVec4(0.80f, 0.80f, 0.80f, 0.90f);           // 浅灰色按钮
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);     // 灰色悬停
        colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.50f, 0.80f, 1.00f);      // 蓝色激活
        
        // Frame - 现代化输入框样式
        colors[ImGuiCol_FrameBg] = ImVec4(0.85f, 0.85f, 0.85f, 0.95f);           // 浅灰色输入框背景
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.75f, 0.75f, 0.75f, 0.98f);    // 灰色悬停背景
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.50f, 0.80f, 1.00f);     // 蓝色激活背景
        
        // Tabs - 现代化标签页样式
        colors[ImGuiCol_Tab] = ImVec4(0.80f, 0.80f, 0.80f, 0.90f);               // 浅灰色未激活标签
        colors[ImGuiCol_TabHovered] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);         // 灰色悬停标签
        colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.50f, 0.80f, 1.00f);          // 蓝色激活标签
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.85f, 0.85f, 0.85f, 0.90f);        // 未聚焦标签
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.25f, 0.55f, 0.85f, 1.00f);   // 未聚焦激活标签
        
        // Title bar - 现代化标题栏样式
        colors[ImGuiCol_TitleBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);           // 浅灰色标题栏背景
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.50f, 0.80f, 1.00f);      // 蓝色激活标题栏
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);    // 折叠标题栏
        
        // Menu bar - 现代化菜单栏样式
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);          // 灰色菜单栏背景
        
        // Scrollbar - 现代化滚动条样式
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.60f);         // 滚动条背景
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.60f, 0.60f, 0.60f, 0.70f);       // 灰色滚动条滑块
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.50f, 0.50f, 0.50f, 0.90f); // 深灰色悬停滑块
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.20f, 0.50f, 0.80f, 1.00f);  // 蓝色激活滑块
        
        // Check box - 现代化勾选标记
        colors[ImGuiCol_CheckMark] = ImVec4(0.20f, 0.50f, 0.80f, 1.00f);           // 蓝色勾选标记
        
        // Slider - 现代化滑块样式
        colors[ImGuiCol_SliderGrab] = ImVec4(0.20f, 0.50f, 0.80f, 1.00f);           // 蓝色滑块
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.60f, 0.90f, 1.00f);      // 亮蓝色激活滑块
        
        // Separator - 现代化分隔线样式
        colors[ImGuiCol_Separator] = ImVec4(0.70f, 0.70f, 0.70f, 0.40f);           // 浅灰色分隔线
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.60f, 0.60f, 0.60f, 0.70f);     // 灰色悬停分隔线
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.20f, 0.50f, 0.80f, 1.00f);      // 蓝色激活分隔线
        
        // Resize grip - 现代化调整大小手柄
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.20f, 0.50f, 0.80f, 0.30f);           // 蓝色调整手柄
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.30f, 0.60f, 0.90f, 0.80f);    // 亮蓝色悬停手柄
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.70f, 1.00f, 1.00f);     // 亮青色激活手柄
        
        // Plot - 现代化图表颜色
        colors[ImGuiCol_PlotLines] = ImVec4(0.20f, 0.50f, 0.80f, 1.00f);            // 蓝色线条
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.15f, 0.40f, 0.70f, 1.00f);     // 深蓝色悬停线条
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.25f, 0.55f, 0.85f, 1.00f);         // 亮蓝色直方图
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.15f, 0.40f, 0.70f, 1.00f);  // 深蓝色悬停直方图
        
        // Table - 现代化表格样式
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.20f, 0.50f, 0.80f, 0.90f);        // 蓝色表头
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.70f, 0.70f, 0.70f, 0.50f);     // 浅灰色强边框
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.80f, 0.80f, 0.80f, 0.30f);      // 更浅灰色弱边框
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);            // 透明行背景
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.90f, 0.90f, 0.90f, 0.80f);         // 浅灰色交替行背景
        
        // Drag and drop - 现代化拖拽目标
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.20f, 0.50f, 0.80f, 0.80f);        // 蓝色拖拽目标
        
        // Navigation - 现代化导航高亮
        colors[ImGuiCol_NavHighlight] = ImVec4(0.20f, 0.50f, 0.80f, 0.80f);          // 蓝色导航高亮
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.10f, 0.10f, 0.10f, 0.70f);  // 深灰色窗口导航高亮
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.70f);      // 浅灰色半透明背景
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.85f, 0.85f, 0.85f, 0.80f);      // 灰白色半透明背景
    }
    
    // SUBSTANCE赛博朋克主题颜色
    void SetupSubstanceCyberpunkColors() {
        auto& colors = ImGui::GetStyle().Colors;
        
        // SUBSTANCE赛博朋克主题颜色
        colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.95f);       // 深黑色窗口背景
        colors[ImGuiCol_ChildBg] = ImVec4(0.08f, 0.08f, 0.15f, 0.95f);        // 稍亮的子窗口背景
        colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.10f, 0.20f, 0.98f);        // 弹出窗口背景
        
        // Borders - 更细的边框，使用霓虹色调
        colors[ImGuiCol_Border] = ImVec4(0.80f, 0.00f, 0.80f, 0.50f);         // 紫色边框
        colors[ImGuiCol_BorderShadow] = ImVec4(0.80f, 0.00f, 0.80f, 0.20f);    // 紫色阴影
        
        // Text - 霓虹色调文本
        colors[ImGuiCol_Text] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);            // 青色文本
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);     // 灰色禁用文本
        
        // Headers - 赛博朋克强调色
        colors[ImGuiCol_Header] = ImVec4(0.80f, 0.00f, 0.80f, 0.90f);           // 紫色标题
        colors[ImGuiCol_HeaderHovered] = ImVec4(1.00f, 0.00f, 1.00f, 1.00f);    // 亮紫色悬停效果
        colors[ImGuiCol_HeaderActive] = ImVec4(0.60f, 0.00f, 0.60f, 1.00f);     // 深紫色激活效果
        
        // Buttons - 赛博朋克按钮样式
        colors[ImGuiCol_Button] = ImVec4(0.15f, 0.15f, 0.25f, 0.90f);           // 深灰色按钮
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.25f, 0.35f, 1.00f);     // 亮灰色悬停
        colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.00f, 0.80f, 1.00f);      // 紫色激活
        
        // Frame - 赛博朋克输入框样式
        colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.25f, 0.95f);           // 深灰色输入框背景
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.35f, 0.98f);    // 亮灰色悬停背景
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.80f, 0.00f, 0.80f, 1.00f);     // 紫色激活背景
        
        // Tabs - 赛博朋克标签页样式
        colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.25f, 0.90f);               // 深灰色未激活标签
        colors[ImGuiCol_TabHovered] = ImVec4(0.25f, 0.25f, 0.35f, 1.00f);         // 亮灰色悬停标签
        colors[ImGuiCol_TabActive] = ImVec4(0.80f, 0.00f, 0.80f, 1.00f);          // 紫色激活标签
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.10f, 0.10f, 0.20f, 0.90f);        // 未聚焦标签
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.60f, 0.00f, 0.60f, 1.00f);   // 未聚焦激活标签
        
        // Title bar - 赛博朋克标题栏样式
        colors[ImGuiCol_TitleBg] = ImVec4(0.05f, 0.05f, 0.10f, 1.00f);           // 深黑色标题栏背景
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.80f, 0.00f, 0.80f, 1.00f);      // 紫色激活标题栏
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.03f, 0.03f, 0.05f, 1.00f);    // 折叠标题栏
        
        // Menu bar - 赛博朋克菜单栏样式
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.08f, 0.08f, 0.15f, 1.00f);          // 稍亮的菜单栏背景
        
        // Scrollbar - 赛博朋克滚动条样式
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.03f, 0.03f, 0.05f, 0.60f);         // 滚动条背景
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.00f, 0.80f, 0.70f);       // 紫色滚动条滑块
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.00f, 0.00f, 1.00f, 0.90f); // 亮紫色悬停滑块
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.60f, 0.00f, 0.60f, 1.00f);  // 深紫色激活滑块
        
        // Check box - 赛博朋克勾选标记
        colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);           // 青色勾选标记
        
        // Slider - 赛博朋克滑块样式
        colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);           // 青色滑块
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 0.80f, 0.80f, 1.00f);      // 深青色激活滑块
        
        // Separator - 赛博朋克分隔线样式
        colors[ImGuiCol_Separator] = ImVec4(0.80f, 0.00f, 0.80f, 0.40f);           // 紫色分隔线
        colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00f, 0.00f, 1.00f, 0.70f);     // 亮紫色悬停分隔线
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);      // 青色激活分隔线
        
        // Resize grip - 赛博朋克调整大小手柄
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 1.00f, 1.00f, 0.30f);           // 青色调整手柄
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.80f);    // 亮青色悬停手柄
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.80f, 0.00f, 0.80f, 1.00f);     // 紫色激活手柄
        
        // Plot - 赛博朋克图表颜色
        colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);            // 青色线条
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.80f, 0.00f, 0.80f, 1.00f);     // 紫色悬停线条
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);         // 青色直方图
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.80f, 0.00f, 0.80f, 1.00f);  // 紫色悬停直方图
        
        // Table - 赛博朋克表格样式
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.80f, 0.00f, 0.80f, 0.90f);        // 紫色表头
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.80f, 0.00f, 0.80f, 0.50f);     // 紫色强边框
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.00f, 1.00f, 1.00f, 0.30f);      // 青色弱边框
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);            // 透明行背景
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.10f, 0.10f, 0.20f, 0.80f);         // 深色交替行背景
        
        // Drag and drop - 赛博朋克拖拽目标
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.00f, 1.00f, 1.00f, 0.80f);        // 青色拖拽目标
        
        // Navigation - 赛博朋克导航高亮
        colors[ImGuiCol_NavHighlight] = ImVec4(0.00f, 1.00f, 1.00f, 0.80f);          // 青色导航高亮
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.00f, 1.00f, 1.00f, 0.70f);  // 青色窗口导航高亮
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.80f);      // 黑色半透明背景
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.02f, 0.02f, 0.04f, 0.90f);      // 深黑色半透明背景
    }
    
    // 通用样式设置
    void SetupStyle() {
        ImGuiStyle& style = ImGui::GetStyle();
        
        // 更现代的布局设置
        style.WindowPadding = ImVec2(14, 12);         // 更舒适的窗口填充
        style.FramePadding = ImVec2(10, 5);          // 更现代化的控件填充
        style.ItemSpacing = ImVec2(12, 8);           // 更舒适的项目间距
        style.ItemInnerSpacing = ImVec2(8, 8);       // 更合理的内部间距
        style.IndentSpacing = 24;                     // 更现代的缩进
        
        // 触摸设备支持
        style.TouchExtraPadding = ImVec2(0, 0);
        
        // 更细的滚动条，更现代的外观
        style.ScrollbarSize = 10;                   // 稍微宽一点的滚动条，更好点击
        style.ScrollbarRounding = 16;                 // 更圆润的滚动条
        
        // 更明显的滑块
        style.GrabMinSize = 14;                      // 更大的滑块，更好点击
        style.GrabRounding = 10;                     // 更圆润的滑块
        
        // 更细的边框，更现代的外观
        style.WindowBorderSize = 1;                 // 细边框
        style.ChildBorderSize = 1;                  // 细边框
        style.PopupBorderSize = 1;                  // 细边框
        style.FrameBorderSize = 1;
        
        // 更圆润的设计，现代感更强
        style.WindowRounding = 12;                   // 更圆润的窗口边角
        style.ChildRounding = 10;                    // 更圆润的子窗口
        style.FrameRounding = 8;                     // 更圆润的控件
        style.PopupRounding = 12;                    // 更圆润的弹出窗口
        style.TabRounding = 8;                       // 更圆润的标签页
        
        // 更现代的对齐方式
        style.WindowTitleAlign = ImVec2(0.05f, 0.5f); // 标题稍微右移，更美观
        style.ButtonTextAlign = ImVec2(0.5f, 0.5f);   // 按钮文本居中
        style.SelectableTextAlign = ImVec2(0.05f, 0.5f); // 列表项稍微右移
        
        // 现代显示器支持
        style.DisplaySafeAreaPadding = ImVec2(4, 4);
        
        // 更高质量的抗锯齿
        style.AntiAliasedLines = true;
        style.AntiAliasedFill = true;
        
        // 更平滑的曲线和圆形
        style.CurveTessellationTol = 0.8f;         // 更高的曲线质量
        style.CircleTessellationMaxError = 0.3f;     // 更高的圆形质量
        
        // 菜单按钮位置
        style.WindowMenuButtonPosition = ImGuiDir_Right;
        
        // 更现代的颜色混合模式
        style.Alpha = 1.0f;
        
        // 启用渐变效果（ImGui 1.89+ 支持）
        style.DisabledAlpha = 0.60f;                 // 禁用控件透明度
        
        // 更现代的窗口阴影效果（如果支持）
        // 注意：ImGui 核心不支持阴影，但一些分支或扩展支持
        // style.WindowShadowSize = 0;
    }
    
    // 恢复默认样式
    void RestoreDefaultStyle() {
        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        style = ImGuiStyle();
    }
    
    // 渲染主题选择器
    void RenderThemeSelector() {
        if (ImGui::BeginCombo("主题选择", GetThemeName(currentTheme))) {
            if (ImGui::Selectable("默认主题", currentTheme == ThemeType::DEFAULT)) {
                ApplyTheme(ThemeType::DEFAULT);
            }
            if (ImGui::Selectable("现代主题", currentTheme == ThemeType::SUBSTANCE_MODERN)) {
                ApplyTheme(ThemeType::SUBSTANCE_MODERN);
            }
            if (ImGui::Selectable("深色主题", currentTheme == ThemeType::SUBSTANCE_DARK)) {
                ApplyTheme(ThemeType::SUBSTANCE_DARK);
            }
            if (ImGui::Selectable("亮色主题", currentTheme == ThemeType::SUBSTANCE_LIGHT)) {
                ApplyTheme(ThemeType::SUBSTANCE_LIGHT);
            }
            if (ImGui::Selectable("赛博朋克主题", currentTheme == ThemeType::SUBSTANCE_CYBERPUNK)) {
                ApplyTheme(ThemeType::SUBSTANCE_CYBERPUNK);
            }
            ImGui::EndCombo();
        }
    }
    
    // 获取主题名称
    const char* GetThemeName(ThemeType theme) {
        switch (theme) {
        case ThemeType::DEFAULT:
            return "默认主题";
        case ThemeType::SUBSTANCE_MODERN:
            return "现代主题";
        case ThemeType::SUBSTANCE_DARK:
            return "深色主题";
        case ThemeType::SUBSTANCE_LIGHT:
            return "亮色主题";
        case ThemeType::SUBSTANCE_CYBERPUNK:
            return "赛博朋克主题";
        default:
            return "未知主题";
        }
    }
    
    // 获取当前主题
    ThemeType GetTheme() {
        return currentTheme;
    }
    
    // 设置主题
    void SetTheme(ThemeType theme) {
        ApplyTheme(theme);
    }
}