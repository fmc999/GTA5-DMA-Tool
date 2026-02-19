#pragma once
#include <string>
#include <vector>

// 菜单页面类型枚举
enum class MenuPage {
    MAIN,           // 主菜单
    PLAYER,         // 玩家功能
    WEAPON,         // 武器功能
    TELEPORT,       // 传送功能
    VEHICLE,        // 载具功能
    TIME,           // 时间控制
    HEIST_DIVIDEND, // 抢劫分红
    SETTINGS        // 设置
};

// 菜单项结构
struct MenuItem {
    std::string name;           // 显示名称
    MenuPage targetPage;        // 目标页面
    bool isEnabled;             // 是否启用
};

// 菜单管理器类
class MenuManager {
public:
    static MenuManager& GetInstance();
    
    // 菜单显示开关 - 允许用户控制每个菜单的显示状态
    static inline bool bShowPlayerMenu = true;
    static inline bool bShowWeaponMenu = true;
    static inline bool bShowTeleportMenu = true;
    static inline bool bShowVehicleMenu = true;
    static inline bool bShowTimeMenu = true;
    static inline bool bShowHeistDividendMenu = true;
    static inline bool bShowSettingsMenu = true;
    
    // 获取当前页面
    MenuPage GetCurrentPage() const { return currentPage; }
    
    // 设置当前页面
    void SetCurrentPage(MenuPage page) { currentPage = page; }
    
    // 切换到指定页面
    void SwitchToPage(MenuPage page);
    
    // 返回上一页
    void GoBack();
    
    // 获取页面历史
    const std::vector<MenuPage>& GetPageHistory() const { return pageHistory; }
    
    // 渲染当前页面
    void RenderCurrentPage();
    
    // 渲染主菜单
    void RenderMainMenu();
    void RenderMainMenuContent();
    
    // 渲染玩家功能页面
    void RenderPlayerPage();
    void RenderPlayerPageContent();
    
    // 渲染武器功能页面
    void RenderWeaponPage();
    void RenderWeaponPageContent();
    
    // 渲染传送功能页面
    void RenderTeleportPage();
    void RenderTeleportPageContent();
    
    // 渲染载具功能页面
    void RenderVehiclePage();
    void RenderVehiclePageContent();
    
    // 渲染时间控制页面
    void RenderTimePage();
    void RenderTimePageContent();
    
    // 渲染设置页面
    void RenderSettingsPage();
    void RenderSettingsPageContent();
    
    // 渲染抢劫任务分红页面
    void RenderHeistDividendPage();
    void RenderHeistDividendPageContent();
    
    // 渲染返回按钮
    void RenderBackButton();
    
    // 渲染菜单项按钮
    bool RenderMenuItem(const std::string& label, MenuPage targetPage, bool enabled = true);
    
    // 渲染页面标题
    void RenderPageTitle(const std::string& title);
    
    // 设置页面窗口大小和位置
    void SetupPageWindow(const ImVec2& size = ImVec2(500, 600));
    
private:
    MenuManager() : currentPage(MenuPage::MAIN) {}
    
    MenuPage currentPage;
    std::vector<MenuPage> pageHistory;
    
    // 渲染页面通用样式
    void ApplyPageStyle();
    void CleanupPageStyle();
};