#pragma once
#include "imgui.h"

// 主题枚举
enum class ThemeType {
    DEFAULT,
    SUBSTANCE_MODERN,
    SUBSTANCE_DARK,
    SUBSTANCE_LIGHT,
    SUBSTANCE_CYBERPUNK
};

namespace ImGuiToolStyle {
    // 应用工具风格的主题
    void ApplyToolStyle();
    
    // 应用指定主题
    void ApplyTheme(ThemeType theme);
    
    // 获取当前主题
    ThemeType GetCurrentTheme();
    
    // 默认主题颜色
    void SetupDefaultColors();
    
    // SUBSTANCE现代主题颜色
    void SetupSubstanceModernColors();
    
    // SUBSTANCE深色主题颜色
    void SetupSubstanceDarkColors();
    
    // SUBSTANCE亮色主题颜色
    void SetupSubstanceLightColors();
    
    // SUBSTANCE赛博朋克主题颜色
    void SetupSubstanceCyberpunkColors();
    
    // 设置自定义样式
    void SetupStyle();
    
    // 恢复默认样式
    void RestoreDefaultStyle();
    
    // 渲染主题选择器
    void RenderThemeSelector();
    
    // 获取主题名称
    const char* GetThemeName(ThemeType theme);
    
    // 获取当前主题
    ThemeType GetTheme();
    
    // 设置主题
    void SetTheme(ThemeType theme);
}