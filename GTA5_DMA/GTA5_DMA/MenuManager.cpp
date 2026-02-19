#include "pch.h"
#include "MenuManager.h"
#include "MyMenu.h"
#include "Dev.h"
#include "InputManager.h"
#include "Offsets.h"
#include "ImGuiToolStyle.h"
#include <time.h>

// 包含所有功能模块
#include "GodMode.h"
#include "WeaponInspector.h"
#include "Teleport.h"
#include "VehicleEditor.h"
#include "TimeControl.h"
#include "Ragdoll.h"
#include "PlayerSpeed.h"
#include "Invisibility.h"
#include "NoCollision.h"
#include "PlayerChaser.h"
#include "RefreshHealth.h"
#include "NoWanted.h"
#include "HeistDividend.h"
#include "ArmorManager.h"
#include "HealthManager.h"

// 将英文月份转换为中文
std::string GetChineseMonth(const std::string& month) {
    if (month == "Jan") return "1";
    if (month == "Feb") return "2";
    if (month == "Mar") return "3";
    if (month == "Apr") return "4";
    if (month == "May") return "5";
    if (month == "Jun") return "6";
    if (month == "Jul") return "7";
    if (month == "Aug") return "8";
    if (month == "Sep") return "9";
    if (month == "Oct") return "10";
    if (month == "Nov") return "11";
    if (month == "Dec") return "12";
    return month;
}

// 将英文日期格式转换为中文日期格式
std::string GetChineseCompileTime() {
    std::string date_str = __DATE__;
    std::string time_str = __TIME__;
    
    // 解析英文日期格式: "MMM DD YYYY"
    std::istringstream iss(date_str);
    std::string month, day, year;
    iss >> month >> day >> year;
    
    // 移除日期中的前导空格
    day.erase(0, day.find_first_not_of(" "));
    
    // 构建中文日期格式: "YYYY年MM月DD日 HH:MM:SS"
    return year + "年" + GetChineseMonth(month) + "月" + day + "日 " + time_str;
}

// 功能组件结构体，模仿SUBSTANCE UI的FunctionWidget
struct FunctionWidget {
    std::string ID;
    std::string Title;
    std::string Description;
    ImVec2 Size;
    ImVec4 BackgroundColor;
    ImVec4 TitleColor;
    ImVec4 DescriptionColor;
    ImVec4 BorderColor;
    ImVec4 OnColor;
    ImVec4 OffColor;
    bool Checked;
    float MarginX;
    float MarginY;
    float CornerSize;
    float LineThickness;
    float BorderPercent;
    bool Animating;
    float BorderOffset;
    float AnimationSpeed;
    std::string BottomRightIconName;
    ImVec4 BottomRightIconBgColor;
    bool IconButtonVisible;
    float IconButtonRounding;
    float IconButtonSize;
    bool Enabled;

    // 构造函数
    FunctionWidget(const std::string& id, const std::string& title, const std::string& desc, ImVec2 size)
        : ID(id), Title(title), Description(desc), Size(size),
          BackgroundColor(ImVec4(0.15f, 0.15f, 0.15f, 1.0f)),
          TitleColor(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)),
          DescriptionColor(ImVec4(0.8f, 0.8f, 0.8f, 1.0f)),
          BorderColor(ImVec4(1, 1, 1, 1)),
          OnColor(ImVec4(0.3f, 1.0f, 0.3f, 1.0f)),
          OffColor(ImVec4(1.0f, 0.3f, 0.3f, 1.0f)),
          Checked(false),
          MarginX(15.0f),
          MarginY(10.0f),
          CornerSize(15.0f),
          LineThickness(1.0f),
          BorderPercent(0.5f),
          Animating(false),
          BorderOffset(0.0f),
          AnimationSpeed(0.5f),
          BottomRightIconName(""),
          BottomRightIconBgColor(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)),
          IconButtonVisible(true),
          IconButtonRounding(5.0f),
          IconButtonSize(20.0f),
          Enabled(true) {}

    // 渲染函数
    bool Render() {
        ImGui::BeginChild(("Section" + ID).c_str(), ImVec2(Size.x, Size.y + 1), ImGuiWindowFlags_NoScrollbar);

        // 确保BorderPercent在0-1之间
        BorderPercent = std::max(0.0f, std::min(1.0f, BorderPercent));

        // 调整颜色如果组件被禁用
        ImVec4 bgColor = BackgroundColor;
        ImVec4 titleColor = TitleColor;
        ImVec4 descColor = DescriptionColor;
        ImVec4 borderColor = BorderColor;
        ImVec4 onColor = OnColor;
        ImVec4 offColor = OffColor;
        ImVec4 iconBgColor = BottomRightIconBgColor;

        if (!Enabled) {
            bgColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
            ImVec4 grayLight = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
            titleColor = grayLight;
            descColor = grayLight;
            borderColor = grayLight;
            onColor = grayLight;
            offColor = grayLight;
            iconBgColor = grayLight;
        }

        ImVec2 widgetMin = ImGui::GetCursorScreenPos();
        ImVec2 widgetMax = ImVec2(widgetMin.x + Size.x, widgetMin.y + Size.y);
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        // 绘制背景
        drawList->AddRectFilled(widgetMin, widgetMax, ImGui::ColorConvertFloat4ToU32(bgColor));

        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 localMin = ImVec2(widgetMin.x - windowPos.x, widgetMin.y - windowPos.y);
        ImVec2 localMax = ImVec2(widgetMax.x - windowPos.x, widgetMax.y - windowPos.y);

        // 计算正确的文字换行位置，使用相对于当前窗口的坐标
        float wrapPos = MarginX + (Size.x - 2 * MarginX);
        ImGui::PushTextWrapPos(wrapPos);

        // 标题
        ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY));
        ImVec4 actualTitleColor = Checked ? titleColor : descColor;
        ImGui::TextColored(actualTitleColor, Title.c_str());
        ImVec2 titleSize = ImGui::CalcTextSize(Title.c_str());
        float lineHeight = ImGui::GetTextLineHeight();
        float descriptionTopOffset = lineHeight + (MarginY * 0.5f);

        // 描述
        ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY + descriptionTopOffset));
        ImGui::TextColored(descColor, Description.c_str());

        // ON/OFF 状态
        std::string onOffText = Checked ? "ON" : "OFF";
        ImVec2 onOffSize = ImGui::CalcTextSize(onOffText.c_str());

        float bottomY = localMax.y - MarginY - onOffSize.y;
        float bottomX = localMin.x + MarginX;
        ImGui::SetCursorPos(ImVec2(bottomX, bottomY));
        ImVec4 onOffVec = Checked ? onColor : offColor;
        ImGui::TextColored(onOffVec, onOffText.c_str());

        ImGui::PopTextWrapPos();

        // 边框
        float targetAlpha = Checked ? 1.0f : 0.0f;
        float currentBorderAlpha = targetAlpha;
        ImVec4 adjustedBorderColor = ImVec4(borderColor.x, borderColor.y, borderColor.z, borderColor.w * currentBorderAlpha);
        ImU32 borderColU32 = ImGui::ColorConvertFloat4ToU32(adjustedBorderColor);

        if (currentBorderAlpha > 0.01f) {
            // 绘制静态边框
            float width = Size.x;
            float height = Size.y;
            float cSize = CornerSize;

            // 计算水平线条
            float totalLengthX = width;
            float initialGapX = totalLengthX - 2 * cSize;
            float gapActualX = initialGapX * (1.0f - BorderPercent);
            float expandX = (initialGapX - gapActualX) / 2.0f;

            float leftLineEnd = widgetMin.x + cSize + expandX;
            float rightLineStart = widgetMax.x - cSize - expandX;

            // 计算垂直线条
            float totalLengthY = height;
            float initialGapY = totalLengthY - 2 * cSize;
            float gapActualY = initialGapY * (1.0f - BorderPercent);
            float expandY = (initialGapY - gapActualY) / 2.0f;

            float topLineEnd = widgetMin.y + cSize + expandY;
            float bottomLineStart = widgetMax.y - cSize - expandY;

            // 绘制边框线
            drawList->AddLine(ImVec2(widgetMin.x, widgetMin.y), ImVec2(leftLineEnd, widgetMin.y), borderColU32, LineThickness);
            drawList->AddLine(ImVec2(rightLineStart, widgetMin.y), ImVec2(widgetMax.x, widgetMin.y), borderColU32, LineThickness);

            drawList->AddLine(ImVec2(widgetMin.x, widgetMax.y), ImVec2(leftLineEnd, widgetMax.y), borderColU32, LineThickness);
            drawList->AddLine(ImVec2(rightLineStart, widgetMax.y), ImVec2(widgetMax.x, widgetMax.y), borderColU32, LineThickness);

            drawList->AddLine(ImVec2(widgetMin.x, widgetMin.y), ImVec2(widgetMin.x, topLineEnd), borderColU32, LineThickness);
            drawList->AddLine(ImVec2(widgetMin.x, bottomLineStart), ImVec2(widgetMin.x, widgetMax.y), borderColU32, LineThickness);

            drawList->AddLine(ImVec2(widgetMax.x - 1, widgetMin.y), ImVec2(widgetMax.x - 1, topLineEnd), borderColU32, LineThickness);
            drawList->AddLine(ImVec2(widgetMax.x - 1, bottomLineStart), ImVec2(widgetMax.x - 1, widgetMax.y), borderColU32, LineThickness);
        }

        // 图标按钮区域（方形，大小为IconButtonSize）
        ImVec2 iconBoxMin = ImVec2(widgetMax.x - MarginX - IconButtonSize, widgetMax.y - MarginY - IconButtonSize);
        ImVec2 iconBoxMax = ImVec2(iconBoxMin.x + IconButtonSize, iconBoxMin.y + IconButtonSize);

        // 绘制图标如果存在且可见
        if (IconButtonVisible) {
            ImU32 iconBgU32 = ImGui::ColorConvertFloat4ToU32(iconBgColor);
            drawList->AddRectFilled(iconBoxMin, iconBoxMax, iconBgU32, IconButtonRounding);

            // 这里可以添加图标绘制逻辑
        }

        // 整个组件的不可见按钮
        ImGui::SetCursorScreenPos(widgetMin);
        bool clicked = false;
        if (ImGui::InvisibleButton(ID.c_str(), Size)) {
            ImVec2 mousePos = ImGui::GetIO().MousePos;
            bool insideIcon = false;
            if (IconButtonVisible) {
                insideIcon = (mousePos.x >= iconBoxMin.x && mousePos.x < iconBoxMax.x &&
                              mousePos.y >= iconBoxMin.y && mousePos.y < iconBoxMax.y);
            }

            if (insideIcon && Enabled) {
                // 点击图标按钮
                // 这里可以添加图标按钮点击逻辑
            }
            else if (Enabled) {
                // 点击组件其他部分
                Checked = !Checked;
                clicked = true;
            }
        }

        ImGui::EndChild();
        return clicked;
    }
};

// 全局功能组件列表
std::vector<FunctionWidget> g_FunctionWidgets;

MenuManager& MenuManager::GetInstance()
{
    static MenuManager instance;
    return instance;
}

// 初始化功能组件
void InitializeFunctionWidgets() {
    g_FunctionWidgets.clear();
    
    // 设置组件颜色为红色主题，使用半透明实现毛玻璃效果
    ImVec4 backgroundColor = ImVec4(0.25f, 0.25f, 0.25f, 0.65f);
    ImVec4 titleColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 descriptionColor = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
    ImVec4 borderColor = ImVec4(1.0f, 0.3f, 0.3f, 1.0f); // 红色边框
    ImVec4 onColor = ImVec4(0.3f, 1.0f, 0.3f, 1.0f);
    ImVec4 offColor = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
    
    // 添加融合器模式菜单
    FunctionWidget fusionModeWidget("func.fusion_mode", "融合器模式", "FMCYYDS", ImVec2(200, 100));
    fusionModeWidget.BackgroundColor = backgroundColor;
    fusionModeWidget.TitleColor = titleColor;
    fusionModeWidget.DescriptionColor = descriptionColor;
    fusionModeWidget.BorderColor = borderColor;
    fusionModeWidget.OnColor = onColor;
    fusionModeWidget.OffColor = offColor;
    fusionModeWidget.Checked = MyMenu::bFusionMode;
    fusionModeWidget.BorderPercent = 0.3f;
    fusionModeWidget.Animating = true;
    g_FunctionWidgets.push_back(fusionModeWidget);
    
    // 添加常用功能菜单
    FunctionWidget commonFunctionsWidget("func.common_functions", "常用功能", "包含玩家无敌、载具无敌等常用功能", ImVec2(200, 100));
    commonFunctionsWidget.BackgroundColor = backgroundColor;
    commonFunctionsWidget.TitleColor = titleColor;
    commonFunctionsWidget.DescriptionColor = descriptionColor;
    commonFunctionsWidget.BorderColor = borderColor;
    commonFunctionsWidget.OnColor = onColor;
    commonFunctionsWidget.OffColor = offColor;
    commonFunctionsWidget.Checked = false;
    commonFunctionsWidget.BorderPercent = 0.3f;
    commonFunctionsWidget.Animating = true;
    g_FunctionWidgets.push_back(commonFunctionsWidget);
    
    // 添加武器菜单
    FunctionWidget weaponWidget("func.weapon", "武器功能", "修改武器属性和功能", ImVec2(200, 100));
    weaponWidget.BackgroundColor = backgroundColor;
    weaponWidget.TitleColor = titleColor;
    weaponWidget.DescriptionColor = descriptionColor;
    weaponWidget.BorderColor = borderColor;
    weaponWidget.OnColor = onColor;
    weaponWidget.OffColor = offColor;
    weaponWidget.Checked = false;
    weaponWidget.BorderPercent = 0.3f;
    weaponWidget.Animating = true;
    g_FunctionWidgets.push_back(weaponWidget);
    
    FunctionWidget teleportWidget("func.teleport", "传送功能", "快速传送到指定位置", ImVec2(200, 100));
    teleportWidget.BackgroundColor = backgroundColor;
    teleportWidget.TitleColor = titleColor;
    teleportWidget.DescriptionColor = descriptionColor;
    teleportWidget.BorderColor = borderColor;
    teleportWidget.OnColor = onColor;
    teleportWidget.OffColor = offColor;
    teleportWidget.Checked = Teleport::bEnable;
    teleportWidget.BorderPercent = 0.3f;
    teleportWidget.Animating = true;
    g_FunctionWidgets.push_back(teleportWidget);
    
    FunctionWidget vehicleEditorWidget("func.vehicle_editor", "载具编辑器", "修改载具属性和功能", ImVec2(200, 100));
    vehicleEditorWidget.BackgroundColor = backgroundColor;
    vehicleEditorWidget.TitleColor = titleColor;
    vehicleEditorWidget.DescriptionColor = descriptionColor;
    vehicleEditorWidget.BorderColor = borderColor;
    vehicleEditorWidget.OnColor = onColor;
    vehicleEditorWidget.OffColor = offColor;
    vehicleEditorWidget.BorderPercent = 0.3f;
    vehicleEditorWidget.Animating = true;
    g_FunctionWidgets.push_back(vehicleEditorWidget);
    
    FunctionWidget timeControlWidget("func.time_control", "时间控制", "调整游戏内时间", ImVec2(200, 100));
    timeControlWidget.BackgroundColor = backgroundColor;
    timeControlWidget.TitleColor = titleColor;
    timeControlWidget.DescriptionColor = descriptionColor;
    timeControlWidget.BorderColor = borderColor;
    timeControlWidget.OnColor = onColor;
    timeControlWidget.OffColor = offColor;
    timeControlWidget.Checked = TimeControl::bEnableUI;
    timeControlWidget.BorderPercent = 0.3f;
    timeControlWidget.Animating = true;
    g_FunctionWidgets.push_back(timeControlWidget);
    
    FunctionWidget heistDividendWidget("func.heist_dividend", "抢劫分红", "修改抢劫任务分红比例", ImVec2(200, 100));
    heistDividendWidget.BackgroundColor = backgroundColor;
    heistDividendWidget.TitleColor = titleColor;
    heistDividendWidget.DescriptionColor = descriptionColor;
    heistDividendWidget.BorderColor = borderColor;
    heistDividendWidget.OnColor = onColor;
    heistDividendWidget.OffColor = offColor;
    heistDividendWidget.Checked = HeistDividend::bEnableUI;
    heistDividendWidget.BorderPercent = 0.3f;
    heistDividendWidget.Animating = true;
    g_FunctionWidgets.push_back(heistDividendWidget);

}

void MenuManager::SwitchToPage(MenuPage page)
{
    if (currentPage != page) {
        pageHistory.push_back(currentPage);
        currentPage = page;
    }
}

void MenuManager::GoBack()
{
    if (!pageHistory.empty()) {
        currentPage = pageHistory.back();
        pageHistory.pop_back();
    }
}

void MenuManager::RenderCurrentPage()
{
    // 初始化功能组件（只在首次调用时初始化）
    static bool initialized = false;
    if (!initialized) {
        InitializeFunctionWidgets();
        initialized = true;
    }
    
    ImGui::SetNextWindowSize(ImVec2(1000, 700), ImGuiCond_FirstUseEver);
    ImGui::Begin("FMC GTA5 DMA ", nullptr, ImGuiWindowFlags_NoCollapse);
    
    // 使用SUBSTANCE风格的红色主题
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    
    // 设置主题颜色 - 使用半透明颜色实现毛玻璃效果
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.20f, 0.20f, 0.20f, 0.60f));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.25f, 0.25f, 0.25f, 0.65f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
    
    // 左侧功能组件列表
    float leftSectionWidth = 220.0f;
    ImGui::BeginChild("LeftSection", ImVec2(leftSectionWidth, 0), true, ImGuiWindowFlags_NoScrollbar);
    
    float topMargin = 10.0f;
    float bottomMargin = 10.0f;
    
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + topMargin);
    
    // 渲染功能组件
    for (auto& widget : g_FunctionWidgets) {
        // 确保融合器模式菜单的状态与MyMenu::bFusionMode保持同步
        if (widget.ID == "func.fusion_mode") {
            widget.Checked = MyMenu::bFusionMode;
        }
        // 保持半透明背景颜色，实现毛玻璃效果
        if (widget.Animating) {
            widget.BorderOffset += 1.0f;
            widget.Animating = !(widget.BorderOffset >= 1.0f);
        }
        if (widget.Render()) {
            // 处理组件点击事件
            if (widget.ID == "func.fusion_mode") {
                MyMenu::bFusionMode = widget.Checked;
            }
            else if (widget.ID == "func.teleport") {
                Teleport::bEnable = widget.Checked;
            }
            else if (widget.ID == "func.time_control") {
                TimeControl::bEnableUI = widget.Checked;
            }
            else if (widget.ID == "func.heist_dividend") {
                HeistDividend::bEnableUI = widget.Checked;
                HeistDividend::bEnable.store(widget.Checked);
            }
            else if (widget.ID == "func.weapon") {
                // 武器菜单不需要特殊状态管理
            }
            
            // 确保只有一个组件为Checked状态（互斥），传送菜单和融合器模式除外
            if (widget.Checked && widget.ID != "func.teleport" && widget.ID != "func.fusion_mode") {
                for (auto& otherWidget : g_FunctionWidgets) {
                    if (otherWidget.ID != widget.ID && otherWidget.Checked && otherWidget.ID != "func.teleport" && otherWidget.ID != "func.fusion_mode") {
                        otherWidget.Checked = false;
                        // 同时更新其他组件对应的功能状态
                        if (otherWidget.ID == "func.time_control") {
                            TimeControl::bEnableUI = false;
                        }
                        else if (otherWidget.ID == "func.heist_dividend") {
                            HeistDividend::bEnableUI = false;
                            HeistDividend::bEnable.store(false);
                        }
                        else if (otherWidget.ID == "func.weapon") {
                            // 武器菜单不需要特殊状态管理
                        }
                    }
                }
            }
            else if (widget.ID == "func.fusion_mode") {
                // 融合器模式不受相互排斥影响，确保其状态与MyMenu::bFusionMode同步
                widget.Checked = MyMenu::bFusionMode;
            }
        }
    }
    
    // 添加大红色退出按钮（居中显示）
    ImGui::Spacing();
    ImGui::Spacing();
    
    // 计算按钮居中位置
    float buttonWidth = 180.0f;
    float offsetX = (leftSectionWidth - buttonWidth) * 0.5f;
    
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
    
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.1f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    if (ImGui::Button("退出", ImVec2(buttonWidth, 40))) {
        // 退出程序
        extern bool bAlive;
        bAlive = false;
    }
    ImGui::PopStyleColor(4);
    
    ImGui::Dummy(ImVec2(0, bottomMargin));
    
    ImGui::EndChild();
    
    ImGui::SameLine();
    
    // 右侧详细设置面板
    ImGui::BeginChild("RightSection", ImVec2(0, 0), true);
    
    float marginX = 15.0f;
    float marginY = 10.0f;
    
    ImVec2 childStartPos = ImGui::GetCursorPos();
    ImGui::SetCursorPos(ImVec2(0, childStartPos.y + marginY));
    
    // 顶部标题栏
    float headerHeight = 40.0f;
    ImGui::BeginChild("HeaderBar", ImVec2(0, headerHeight), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
    ImGui::Text("FMC GTA5 DMA ~ SUBSTANCE UI");
    ImGui::PopStyleColor();
    ImGui::EndChild();
    
    // 详细设置区域
    ImGui::BeginChild("SettingsArea", ImVec2(0, 0), true);
    
    // 根据选中的功能组件显示对应的设置（传送菜单除外）
    bool functionSelected = false;
    
    // 优先显示其他功能菜单的内容
    for (const auto& widget : g_FunctionWidgets) {
        if (widget.Checked && widget.ID != "func.teleport") {
            // 跳过融合器模式和退出按钮，先检查其他功能
            if (widget.ID != "func.fusion_mode" && widget.ID != "func.exit") {
                if (widget.ID == "func.common_functions") {
                    ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "常用功能设置");
                    ImGui::Spacing();
                    
                    // 玩家无敌
                    bool playerGodMode = GodMode::bPlayerGodMode.load();
                    if (ImGui::Checkbox("玩家无敌", &playerGodMode)) {
                        GodMode::bPlayerGodMode.store(playerGodMode);
                        GodMode::bRequestedGodmode.store(true);
                    }
                    ImGui::Spacing();
                    
                    // 载具无敌
                    bool vehicleGodMode = GodMode::bVehicleGodMode.load();
                    if (ImGui::Checkbox("载具无敌", &vehicleGodMode)) {
                        GodMode::bVehicleGodMode.store(vehicleGodMode);
                        GodMode::bRequestedGodmode.store(true);
                    }
                    ImGui::Spacing();
                    
                    // 永不被通缉
                    if (ImGui::Checkbox("永不被通缉", &NoWanted::bEnable)) {
                        // 状态已直接修改
                    }
                    ImGui::Spacing();
                    
                    // 隐身功能
                    if (ImGui::Checkbox("隐身功能", &Invisibility::bInvisibilityUI)) {
                        // 状态已直接修改
                    }
                    ImGui::Spacing();
                    
                    // 无碰撞体积
                    if (ImGui::Checkbox("无碰撞体积", &NoCollision::bNoCollisionUI)) {
                        // 状态已直接修改
                    }
                    ImGui::Spacing();
                    
                    // 速度控制
                    if (ImGui::Checkbox("速度控制", &PlayerSpeed::bEnableUI)) {
                        // 状态已直接修改
                    }
                    if (PlayerSpeed::bEnableUI) {
                        ImGui::Indent();
                        // 野兽模式复选框
                        ImGui::Checkbox("野兽模式 (速度1.5)", &PlayerSpeed::bBeastModeUI);
                        
                        // 显示当前速度值
                        ImGui::Text("当前速度: %.1f", PlayerSpeed::playerSpeedUI);
                        
                        // 如果未启用野兽模式，则显示滑块
            if (!PlayerSpeed::bBeastModeUI) {
                ImGui::SliderFloat("人物速度", &PlayerSpeed::playerSpeedUI, 1.0f, 10.0f, "%.2f");
            }
                        ImGui::Unindent();
                    }
                    ImGui::Spacing();
                    
                    // 自动刷新生命值
                    if (ImGui::Checkbox("自动刷新生命值", &RefreshHealth::bEnable)) {
                        // 状态已直接修改
                    }
                    ImGui::Spacing();
                    
                    // 无布娃娃效果 (已固定开启)
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
                    ImGui::Text("无布娃娃效果 (默认开启不关闭)");
                    ImGui::PopStyleColor();
                    ImGui::Spacing();
                    
                    // 显示当前防弹衣值
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
                    ImGui::Text("当前防弹衣: %.0f", ArmorManager::currentArmor);
                    ImGui::PopStyleColor();
                    
                    // 锁定防弹衣
                    if (ImGui::Checkbox("锁定防弹衣 (200)", &ArmorManager::bLockArmor)) {
                        // 状态已直接修改
                    }
                    ImGui::Spacing();
                    
                    // 显示当前生命值
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
                    ImGui::Text("当前生命值: %.0f", HealthManager::currentHealth);
                    ImGui::PopStyleColor();
                    
                    // 锁定生命值
                    if (ImGui::Checkbox("锁定生命值 (200) - 假无敌", &HealthManager::bLockHealth)) {
                        // 状态已直接修改
                    }
                    ImGui::Spacing();
                }
                else if (widget.ID == "func.vehicle_editor") {
                    ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "载具编辑器");
                    ImGui::Spacing();
                    VehicleEditor::RenderContent();
                }
                else if (widget.ID == "func.time_control") {
                    ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "时间控制设置");
                    ImGui::Spacing();
                    ImGui::SliderInt("天", &TimeControl::dayUI, 0, 30);
                    ImGui::SliderInt("时", &TimeControl::hourUI, 0, 23);
                    ImGui::SliderInt("分", &TimeControl::minuteUI, 0, 59);
                    ImGui::SliderInt("秒", &TimeControl::secondUI, 0, 59);
                }
                else if (widget.ID == "func.heist_dividend") {
                    ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "抢劫分红设置");
                    ImGui::Spacing();
                    RenderHeistDividendPageContent();
                }
                else if (widget.ID == "func.weapon") {
                    ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "武器功能设置");
                    ImGui::Spacing();
                    WeaponInspector::RenderContent();
                }
                functionSelected = true;
                break;
            }
        }
    }
    
    // 如果没有选择其他功能，且融合器模式开启，则显示融合器模式设置
    if (!functionSelected) {
        for (const auto& widget : g_FunctionWidgets) {
            if (widget.Checked && widget.ID == "func.fusion_mode") {
                ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "融合器模式设置");
                ImGui::Spacing();
                ImGui::Text("融合模式已启用，将整合多种功能。");
                ImGui::Spacing();
                ImGui::Text("(DMAYA BY FMC)");
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();
                ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "融合模式特点：");
                ImGui::BulletText("整合多种功能到统一界面");
                ImGui::BulletText("优化的用户交互体验");
                ImGui::BulletText("更高效的功能管理");
                functionSelected = true;
                break;
            }
        }
    }
    
    // 如果没有选中任何功能，显示默认信息（传送菜单除外）
    bool anyChecked = false;
    for (const auto& widget : g_FunctionWidgets) {
        if (widget.Checked && widget.ID != "func.teleport") {
            anyChecked = true;
            break;
        }
    }
    
    if (!anyChecked) {
        // 增大字体显示欢迎信息
        ImGui::PushFont(ImGui::GetIO().FontDefault);
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "欢迎使用 FMC GTA5 DMA");
        ImGui::PopFont();
        
        ImGui::Spacing();
        ImGui::Text("请从左侧选择要使用的功能。");
        ImGui::Spacing();
        ImGui::Text("功能列表：");
        ImGui::BulletText("玩家无敌 - 使角色不受伤害");
        ImGui::BulletText("载具无敌 - 使车辆不受伤害");
        ImGui::BulletText("永不被通缉 - 保持通缉等级为零");
        ImGui::BulletText("隐身功能 - 对其他玩家不可见");
        ImGui::BulletText("无碰撞体积 - 穿过物体和玩家");
        ImGui::BulletText("速度控制 - 调整移动速度");
        ImGui::BulletText("传送功能 - 快速移动到指定位置");
        ImGui::BulletText("载具编辑器 - 修改载具属性");
        ImGui::BulletText("时间控制 - 调整游戏内时间");
        ImGui::BulletText("抢劫分红 - 修改任务分红比例");
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        
        // 每日一言
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "📅 每日一言");
        static std::string dailyQuote = "正在获取每日一言...";
        static bool quoteLoaded = false;
        
        if (!quoteLoaded) {
            // 本地随机一言列表
            std::vector<std::string> localQuotes = {
                "人生得意须尽欢，莫使金樽空对月。",
                "天生我材必有用，千金散尽还复来。",
                "长风破浪会有时，直挂云帆济沧海。",
                "山重水复疑无路，柳暗花明又一村。",
                "纸上得来终觉浅，绝知此事要躬行。",
                "问渠那得清如许？为有源头活水来。",
                "落红不是无情物，化作春泥更护花。",
                "春蚕到死丝方尽，蜡炬成灰泪始干。",
                "海内存知己，天涯若比邻。",
                "但愿人长久，千里共婵娟。"
            };
            
            // 随机选择本地一言
            srand(static_cast<unsigned int>(time(nullptr)));
            int randomIndex = rand() % localQuotes.size();
            dailyQuote = localQuotes[randomIndex];
            quoteLoaded = true;
        }
        
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s", dailyQuote.c_str());
        ImGui::Spacing();
        
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "📢 公告");
        ImGui::BulletText("辅机雷达机HOME显示隐藏菜单");
        ImGui::BulletText("此版本支持传承和增强");
        ImGui::BulletText("传承凑合用");
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "版本号：2.78");
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "编译时间：%s", GetChineseCompileTime().c_str());
        ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.0f, 1.0f), "群: 1085350916");
    }
    
    ImGui::EndChild();
    
    ImGui::EndChild();
    
    // 清理样式
    ImGui::PopStyleColor(5);
    ImGui::PopStyleVar();
    
    ImGui::End();
}

void MenuManager::ApplyPageStyle()
{
    // 使用现代化的布局，与ImGuiToolStyle保持一致
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(14, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 8));
    
    // 设置标签页样式 - 使用蓝绿色主题
    ImGui::PushStyleColor(ImGuiCol_Tab, ImVec4(0.18f, 0.22f, 0.28f, 0.90f));
    ImGui::PushStyleColor(ImGuiCol_TabHovered, ImVec4(0.25f, 0.35f, 0.45f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_TabActive, ImVec4(0.20f, 0.60f, 0.80f, 1.00f));
    
    // 设置窗口背景色 - 深灰蓝色主题
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.12f, 0.15f, 0.20f, 0.92f));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.15f, 0.18f, 0.24f, 0.95f));
    
    // 设置边框和分割线颜色 - 蓝灰色
    ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.25f, 0.35f, 0.45f, 0.40f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.25f, 0.35f, 0.45f, 0.30f));
    
    // 设置按钮样式 - 蓝绿色主题
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.55f, 0.75f, 0.90f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.65f, 0.85f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.20f, 0.50f, 0.70f, 1.00f));
    
    // 设置复选框样式 - 蓝绿色勾选标记
    ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.40f, 0.80f, 1.00f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.18f, 0.22f, 0.28f, 0.95f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.22f, 0.28f, 0.35f, 0.98f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.25f, 0.35f, 0.45f, 1.00f));
    
    // 设置文本颜色 - 亮白色带有蓝绿色调
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.95f, 0.98f, 1.00f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImVec4(0.50f, 0.60f, 0.70f, 1.00f));
    
    // 设置标题颜色 - 蓝绿色主题
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.20f, 0.60f, 0.80f, 0.90f));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.30f, 0.70f, 0.90f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.15f, 0.50f, 0.70f, 1.00f));
}

void MenuManager::CleanupPageStyle()
{
    // 弹出所有PushStyleColor和PushStyleVar的调用
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(19); // 19个PushStyleColor调用
}

void MenuManager::RenderMainMenuContent()
{
    // 标题
    RenderPageTitle("主菜单");
    
    ImGui::Separator();
    
    // 融合模式选项
    ImGui::Checkbox("🌐 融合模式", &MyMenu::bFusionMode);
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
    ImGui::Text("(DMAYA BY FMC)");
    ImGui::PopStyleColor();
    
    ImGui::Separator();
    
    // 退出按钮
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.80f, 0.20f, 0.20f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.90f, 0.30f, 0.30f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.70f, 0.10f, 0.10f, 1.0f));
    
    if (ImGui::Button("🚪 退出程序", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
        extern bool bAlive;
        bAlive = false;
    }
    
    ImGui::PopStyleColor(3);
}

void MenuManager::RenderMainMenu()
{
    ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_FirstUseEver);
    ImGui::Begin("FMC GTA5 DMA ", nullptr, ImGuiWindowFlags_NoCollapse);
    RenderMainMenuContent();
    ImGui::End();
}

void MenuManager::RenderPlayerPageContent()
{
    // 标题
    RenderPageTitle("玩家功能");
    
    ImGui::Separator();
    
    // 核心生存功能
    ImGui::TextColored(ImVec4(0.26f, 0.59f, 0.98f, 1.0f), "核心生存功能");
    ImGui::Spacing();
    
    // 玩家无敌
    bool playerGodMode = GodMode::bPlayerGodMode.load();
    bool prevPlayerGodMode = playerGodMode;
    ImGui::Checkbox("玩家无敌", &playerGodMode);
    if (playerGodMode != prevPlayerGodMode) {
        GodMode::bPlayerGodMode.store(playerGodMode);
        GodMode::bRequestedGodmode.store(true);
    }
    
    // 视觉反馈
    if (playerGodMode) {
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
        ImGui::Text("[已开启]");
        ImGui::PopStyleColor();
    }
    
    // 载具无敌
    bool vehicleGodMode = GodMode::bVehicleGodMode.load();
    bool prevVehicleGodMode = vehicleGodMode;
    ImGui::Checkbox("载具无敌", &vehicleGodMode);
    if (vehicleGodMode != prevVehicleGodMode) {
        GodMode::bVehicleGodMode.store(vehicleGodMode);
        GodMode::bRequestedGodmode.store(true);
    }
    
    // 视觉反馈
    if (vehicleGodMode) {
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
        ImGui::Text("[已开启]");
        ImGui::PopStyleColor();
    }
    
    // 永不被通缉
    ImGui::Checkbox("永不被通缉", &NoWanted::bEnable);
    
    // 自动刷新生命值
    ImGui::Checkbox("自动刷新生命值", &RefreshHealth::bEnable);
    if (RefreshHealth::bEnable) {
        ImGui::Indent();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
        ImGui::Text("阈值: %.0f%% (低于此值时自动恢复)", RefreshHealth::HealThresholdPercent * 100.0f);
        ImGui::PopStyleColor();
        ImGui::Unindent();
    }
    
    ImGui::Separator();
    
    // 高级功能
    ImGui::TextColored(ImVec4(0.26f, 0.59f, 0.98f, 1.0f), "高级功能");
    ImGui::Spacing();
    
    // 隐身功能
    ImGui::Checkbox("启用隐身", &Invisibility::bInvisibilityUI);
    if (Invisibility::bInvisibilityUI) {
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
        ImGui::Text("[已开启]");
        ImGui::PopStyleColor();
    }
    
    // 无碰撞体积
    ImGui::Checkbox("无碰撞体积", &NoCollision::bNoCollisionUI);
    if (NoCollision::bNoCollisionUI) {
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
        ImGui::Text("[已开启]");
        ImGui::PopStyleColor();
    }
    
    // 玩家速度控制
    ImGui::Checkbox("启用速度控制", &PlayerSpeed::bEnableUI);
    if (PlayerSpeed::bEnableUI) {
        ImGui::Indent();
        // 野兽模式复选框
        ImGui::Checkbox("野兽模式 (速度1.5)", &PlayerSpeed::bBeastModeUI);
        
        // 显示当前速度值
        ImGui::Text("当前速度: %.1f", PlayerSpeed::playerSpeedUI);
        
        // 如果未启用野兽模式，则显示滑块
        if (!PlayerSpeed::bBeastModeUI) {
            ImGui::SliderFloat("人物速度", &PlayerSpeed::playerSpeedUI, 1.0f, 10.0f, "%.2f");
        }
        ImGui::Unindent();
    }
    
    // 无布娃娃效果 (已固定开启)
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
    ImGui::Text("无布娃娃效果 (已固定开启)");
    ImGui::PopStyleColor();
    
    ImGui::Separator();
    
    // 状态管理
    ImGui::TextColored(ImVec4(0.26f, 0.59f, 0.98f, 1.0f), "状态管理");
    ImGui::Spacing();
    
    // 防弹衣管理
    // 显示当前防弹衣值
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.80f, 0.80f, 0.80f, 1.0f));
    ImGui::Text("当前防弹衣: %.0f", ArmorManager::currentArmor);
    ImGui::PopStyleColor();
    
    // 锁定防弹衣开关
    bool lockArmor = ArmorManager::bLockArmor;
    if (ImGui::Checkbox("锁定防弹衣 (200)", &lockArmor)) {
        ArmorManager::bLockArmor = lockArmor;
    }
    
    // 生命值管理
    // 显示当前生命值
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.80f, 0.80f, 0.80f, 1.0f));
    ImGui::Text("当前生命值: %.0f", HealthManager::currentHealth);
    ImGui::PopStyleColor();
    
    // 锁定生命值开关
    bool lockHealth = HealthManager::bLockHealth;
    if (ImGui::Checkbox("锁定生命值 (200) - 假无敌", &lockHealth)) {
        HealthManager::bLockHealth = lockHealth;
    }
}

void MenuManager::RenderPlayerPage()
{
    SetupPageWindow();
    ImGui::Begin("玩家功能", nullptr, ImGuiWindowFlags_NoCollapse);
    RenderPlayerPageContent();
    RenderBackButton();
    ImGui::End();
}

void MenuManager::RenderWeaponPageContent()
{
    // 标题
    RenderPageTitle("武器功能");
    
    ImGui::Separator();
    
    // 武器检查器
    WeaponInspector::RenderContent();
}

void MenuManager::RenderWeaponPage()
{
    SetupPageWindow();
    ImGui::Begin("武器功能", nullptr, ImGuiWindowFlags_NoCollapse);
    RenderWeaponPageContent();
    RenderBackButton();
    ImGui::End();
}

void MenuManager::RenderTeleportPageContent()
{
    // 标题
    RenderPageTitle("传送功能");
    
    ImGui::Separator();
    
    // 传送功能
    ImGui::Checkbox("启用传送", &Teleport::bEnable);
}

void MenuManager::RenderTeleportPage()
{
    SetupPageWindow();
    ImGui::Begin("传送功能", nullptr, ImGuiWindowFlags_NoCollapse);
    RenderTeleportPageContent();
    RenderBackButton();
    ImGui::End();
}

void MenuManager::RenderVehiclePageContent()
{
    // 标题
    RenderPageTitle("载具功能");
    
    ImGui::Separator();
    
    // 载具编辑器
    VehicleEditor::RenderContent();
}

void MenuManager::RenderVehiclePage()
{
    SetupPageWindow();
    ImGui::Begin("载具功能", nullptr, ImGuiWindowFlags_NoCollapse);
    RenderVehiclePageContent();
    RenderBackButton();
    ImGui::End();
}

void MenuManager::RenderTimePageContent()
{
    // 标题
    RenderPageTitle("时间控制");
    
    ImGui::Separator();
    
    // 时间控制
    ImGui::Checkbox("启用时间控制", &TimeControl::bEnableUI);
    if (TimeControl::bEnableUI) {
        ImGui::Indent();
        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.26f, 0.59f, 0.98f, 1.0f), "时间设置");
        ImGui::SliderInt("天", &TimeControl::dayUI, 0, 30);
        ImGui::SliderInt("时", &TimeControl::hourUI, 0, 23);
        ImGui::SliderInt("分", &TimeControl::minuteUI, 0, 59);
        ImGui::SliderInt("秒", &TimeControl::secondUI, 0, 59);
        ImGui::Spacing();
        ImGui::Unindent();
    }
}

void MenuManager::RenderTimePage()
{
    SetupPageWindow();
    ImGui::Begin("时间控制", nullptr, ImGuiWindowFlags_NoCollapse);
    RenderTimePageContent();
    RenderBackButton();
    ImGui::End();
}

void MenuManager::RenderHeistDividendPageContent()
{
    // 标题
    RenderPageTitle("抢劫任务分红");
    
    ImGui::Separator();
    
    // 启用开关
    if (ImGui::Checkbox("启用抢劫分红修改##heist_enable", &HeistDividend::bEnableUI)) {
        // 状态改变时同步到原子变量
        HeistDividend::bEnable.store(HeistDividend::bEnableUI);
    }
    
    // 分红设置区域
    if (HeistDividend::bEnableUI) {
        // 实时读取并显示分红值
        static std::vector<int> realtimeDividends(8, 0);
        static double lastUpdateTime = 0.0;
        double currentTime = ImGui::GetTime();
        
        // 每0.5秒更新一次实时分红值
        if (currentTime - lastUpdateTime > 0.5f) {
            lastUpdateTime = currentTime;
            
            // 读取当前分红值
            uintptr_t baseAddr = DMA::BaseAddress + 0x03F8E970;
            if (baseAddr) {
                DWORD BytesRead = 0;
                uintptr_t addr1 = 0;
                if (VMMDLL_MemReadEx(DMA::vmh, DMA::PID, baseAddr, (BYTE*)&addr1, sizeof(uintptr_t), &BytesRead, VMMDLL_FLAG_NOCACHE) && BytesRead == sizeof(uintptr_t) && addr1) {
                    // 赌场豪劫分红
                    int value = 0;
                    if (VMMDLL_MemReadEx(DMA::vmh, DMA::PID, addr1 + 0x112828, (BYTE*)&value, sizeof(int), &BytesRead, VMMDLL_FLAG_NOCACHE) && BytesRead == sizeof(int)) {
                        realtimeDividends[0] = value;
                    }
                    if (VMMDLL_MemReadEx(DMA::vmh, DMA::PID, addr1 + 0x112830, (BYTE*)&value, sizeof(int), &BytesRead, VMMDLL_FLAG_NOCACHE) && BytesRead == sizeof(int)) {
                        realtimeDividends[1] = value;
                    }
                    if (VMMDLL_MemReadEx(DMA::vmh, DMA::PID, addr1 + 0x112838, (BYTE*)&value, sizeof(int), &BytesRead, VMMDLL_FLAG_NOCACHE) && BytesRead == sizeof(int)) {
                        realtimeDividends[2] = value;
                    }
                    if (VMMDLL_MemReadEx(DMA::vmh, DMA::PID, addr1 + 0x112840, (BYTE*)&value, sizeof(int), &BytesRead, VMMDLL_FLAG_NOCACHE) && BytesRead == sizeof(int)) {
                        realtimeDividends[3] = value;
                    }
                    
                    // 佩里科岛分红
                    if (VMMDLL_MemReadEx(DMA::vmh, DMA::PID, addr1 + 0x11CFD8, (BYTE*)&value, sizeof(int), &BytesRead, VMMDLL_FLAG_NOCACHE) && BytesRead == sizeof(int)) {
                        realtimeDividends[4] = value;
                    }
                    if (VMMDLL_MemReadEx(DMA::vmh, DMA::PID, addr1 + 0x11CFE0, (BYTE*)&value, sizeof(int), &BytesRead, VMMDLL_FLAG_NOCACHE) && BytesRead == sizeof(int)) {
                        realtimeDividends[5] = value;
                    }
                    if (VMMDLL_MemReadEx(DMA::vmh, DMA::PID, addr1 + 0x11CFE8, (BYTE*)&value, sizeof(int), &BytesRead, VMMDLL_FLAG_NOCACHE) && BytesRead == sizeof(int)) {
                        realtimeDividends[6] = value;
                    }
                    if (VMMDLL_MemReadEx(DMA::vmh, DMA::PID, addr1 + 0x11CFF0, (BYTE*)&value, sizeof(int), &BytesRead, VMMDLL_FLAG_NOCACHE) && BytesRead == sizeof(int)) {
                        realtimeDividends[7] = value;
                    }
                }
            }
        }
        
        // 左右布局
        ImGui::Columns(2, "main_layout", false);
        
        // 左侧：预设按钮（自适应宽度）
        float availableWidth = ImGui::GetContentRegionAvail().x;
        // 增加左侧列的最小宽度，确保按钮文字不会被截断
        float leftColumnWidth = std::max(450.0f, availableWidth * 0.7f);
        ImGui::SetColumnWidth(0, leftColumnWidth);
        
        // 按钮区域
        ImGui::Spacing();
        
        // 全员85按钮
        if (ImGui::Button("赌场全员85##set_all_85", ImVec2(150, 30))) {
            // 确保功能已启用
            HeistDividend::bEnableUI = true;
            HeistDividend::bEnable.store(true);
            
            // 设置赌场豪劫分红为85%
            HeistDividend::casinoDividend1PUI = 85;
            HeistDividend::casinoDividend2PUI = 85;
            HeistDividend::casinoDividend3PUI = 85;
            HeistDividend::casinoDividend4PUI = 85;
            
            HeistDividend::casinoDividend1P.store(85);
            HeistDividend::casinoDividend2P.store(85);
            HeistDividend::casinoDividend3P.store(85);
            HeistDividend::casinoDividend4P.store(85);
            
            // 调用UpdateDividends写入游戏内存，与手动输入框逻辑一致
            HeistDividend::UpdateDividends();
        }
        
        ImGui::SameLine();
        
        // 佩里科岛猎豹全员135按钮
        if (ImGui::Button("🐆 佩里科岛猎豹全员135##set_cayo_leopard_135", ImVec2(180, 30))) {
            // 确保功能已启用
            HeistDividend::bEnableUI = true;
            HeistDividend::bEnable.store(true);
            
            // 设置佩里科岛分红为135%
            HeistDividend::cayoDividend1PUI = 135;
            HeistDividend::cayoDividend2PUI = 135;
            HeistDividend::cayoDividend3PUI = 135;
            HeistDividend::cayoDividend4PUI = 135;
            
            HeistDividend::cayoDividend1P.store(135);
            HeistDividend::cayoDividend2P.store(135);
            HeistDividend::cayoDividend3P.store(135);
            HeistDividend::cayoDividend4P.store(135);
            
            // 调用UpdateDividends写入游戏内存
            HeistDividend::UpdateDividends();
        }
        
        // 新行
        ImGui::Spacing();
        
        // 佩里科岛粉钻全员198按钮
        if (ImGui::Button("💎 佩里科岛粉钻全员198##set_cayo_diamond_198", ImVec2(180, 30))) {
            // 确保功能已启用
            HeistDividend::bEnableUI = true;
            HeistDividend::bEnable.store(true);
            
            // 设置佩里科岛分红为198%
            HeistDividend::cayoDividend1PUI = 198;
            HeistDividend::cayoDividend2PUI = 198;
            HeistDividend::cayoDividend3PUI = 198;
            HeistDividend::cayoDividend4PUI = 198;
            
            HeistDividend::cayoDividend1P.store(198);
            HeistDividend::cayoDividend2P.store(198);
            HeistDividend::cayoDividend3P.store(198);
            HeistDividend::cayoDividend4P.store(198);
            
            // 调用UpdateDividends写入游戏内存
            HeistDividend::UpdateDividends();
        }
        
        // 新行
        ImGui::Spacing();
        
        // 赌场豪劫预设分红按钮
        if (ImGui::Button("🎰 赌场豪劫现金全员75##set_casino_cash_75", ImVec2(180, 30))) {
            // 确保功能已启用
            HeistDividend::bEnableUI = true;
            HeistDividend::bEnable.store(true);
            
            // 设置赌场豪劫分红为75%
            HeistDividend::casinoDividend1PUI = 75;
            HeistDividend::casinoDividend2PUI = 75;
            HeistDividend::casinoDividend3PUI = 75;
            HeistDividend::casinoDividend4PUI = 75;
            
            HeistDividend::casinoDividend1P.store(75);
            HeistDividend::casinoDividend2P.store(75);
            HeistDividend::casinoDividend3P.store(75);
            HeistDividend::casinoDividend4P.store(75);
            
            // 调用UpdateDividends写入游戏内存
            HeistDividend::UpdateDividends();
        }
        
        ImGui::SameLine();
        
        // 赌场豪劫预设分红按钮
        if (ImGui::Button("🎰 赌场豪劫黄金全员90##set_casino_gold_90", ImVec2(180, 30))) {
            // 确保功能已启用
            HeistDividend::bEnableUI = true;
            HeistDividend::bEnable.store(true);
            
            // 设置赌场豪劫分红为90%
            HeistDividend::casinoDividend1PUI = 90;
            HeistDividend::casinoDividend2PUI = 90;
            HeistDividend::casinoDividend3PUI = 90;
            HeistDividend::casinoDividend4PUI = 90;
            
            HeistDividend::casinoDividend1P.store(90);
            HeistDividend::casinoDividend2P.store(90);
            HeistDividend::casinoDividend3P.store(90);
            HeistDividend::casinoDividend4P.store(90);
            
            // 调用UpdateDividends写入游戏内存
            HeistDividend::UpdateDividends();
        }
        
        // 新行
        ImGui::Spacing();
        
        // 佩里科岛预设分红按钮
        if (ImGui::Button("🏝️ 佩里科岛现金全员110##set_cayo_cash_110", ImVec2(180, 30))) {
            // 确保功能已启用
            HeistDividend::bEnableUI = true;
            HeistDividend::bEnable.store(true);
            
            // 设置佩里科岛分红为110%
            HeistDividend::cayoDividend1PUI = 110;
            HeistDividend::cayoDividend2PUI = 110;
            HeistDividend::cayoDividend3PUI = 110;
            HeistDividend::cayoDividend4PUI = 110;
            
            HeistDividend::cayoDividend1P.store(110);
            HeistDividend::cayoDividend2P.store(110);
            HeistDividend::cayoDividend3P.store(110);
            HeistDividend::cayoDividend4P.store(110);
            
            // 调用UpdateDividends写入游戏内存
            HeistDividend::UpdateDividends();
        }
        
        ImGui::SameLine();
        
        // 佩里科岛预设分红按钮
        if (ImGui::Button("🏝️ 佩里科岛艺术品全员150##set_cayo_art_150", ImVec2(180, 30))) {
            // 确保功能已启用
            HeistDividend::bEnableUI = true;
            HeistDividend::bEnable.store(true);
            
            // 设置佩里科岛分红为150%
            HeistDividend::cayoDividend1PUI = 150;
            HeistDividend::cayoDividend2PUI = 150;
            HeistDividend::cayoDividend3PUI = 150;
            HeistDividend::cayoDividend4PUI = 150;
            
            HeistDividend::cayoDividend1P.store(150);
            HeistDividend::cayoDividend2P.store(150);
            HeistDividend::cayoDividend3P.store(150);
            HeistDividend::cayoDividend4P.store(150);
            
            // 调用UpdateDividends写入游戏内存
            HeistDividend::UpdateDividends();
        }
        
        // 新行
        ImGui::Spacing();
        
        // 赌场豪劫更多预设
        if (ImGui::Button("🎰 赌场豪劫现金全员95##set_casino_cash_95", ImVec2(180, 30))) {
            // 确保功能已启用
            HeistDividend::bEnableUI = true;
            HeistDividend::bEnable.store(true);
            
            // 设置赌场豪劫分红为95%
            HeistDividend::casinoDividend1PUI = 95;
            HeistDividend::casinoDividend2PUI = 95;
            HeistDividend::casinoDividend3PUI = 95;
            HeistDividend::casinoDividend4PUI = 95;
            
            HeistDividend::casinoDividend1P.store(95);
            HeistDividend::casinoDividend2P.store(95);
            HeistDividend::casinoDividend3P.store(95);
            HeistDividend::casinoDividend4P.store(95);
            
            // 调用UpdateDividends写入游戏内存
            HeistDividend::UpdateDividends();
        }
        
        ImGui::SameLine();
        
        // 赌场豪劫更多预设
        if (ImGui::Button("🎰 赌场豪劫黄金全员80##set_casino_gold_80", ImVec2(180, 30))) {
            // 确保功能已启用
            HeistDividend::bEnableUI = true;
            HeistDividend::bEnable.store(true);
            
            // 设置赌场豪劫分红为80%
            HeistDividend::casinoDividend1PUI = 80;
            HeistDividend::casinoDividend2PUI = 80;
            HeistDividend::casinoDividend3PUI = 80;
            HeistDividend::casinoDividend4PUI = 80;
            
            HeistDividend::casinoDividend1P.store(80);
            HeistDividend::casinoDividend2P.store(80);
            HeistDividend::casinoDividend3P.store(80);
            HeistDividend::casinoDividend4P.store(80);
            
            // 调用UpdateDividends写入游戏内存
            HeistDividend::UpdateDividends();
        }
        
        // 新行
        ImGui::Spacing();
        
        // 佩里科岛更多预设
        if (ImGui::Button("🏝️ 佩里科岛现金全员120##set_cayo_cash_120", ImVec2(180, 30))) {
            // 确保功能已启用
            HeistDividend::bEnableUI = true;
            HeistDividend::bEnable.store(true);
            
            // 设置佩里科岛分红为120%
            HeistDividend::cayoDividend1PUI = 120;
            HeistDividend::cayoDividend2PUI = 120;
            HeistDividend::cayoDividend3PUI = 120;
            HeistDividend::cayoDividend4PUI = 120;
            
            HeistDividend::cayoDividend1P.store(120);
            HeistDividend::cayoDividend2P.store(120);
            HeistDividend::cayoDividend3P.store(120);
            HeistDividend::cayoDividend4P.store(120);
            
            // 调用UpdateDividends写入游戏内存
            HeistDividend::UpdateDividends();
        }
        
        ImGui::SameLine();
        
        // 佩里科岛更多预设
        if (ImGui::Button("🏝️ 佩里科岛艺术品全员180##set_cayo_art_180", ImVec2(180, 30))) {
            // 确保功能已启用
            HeistDividend::bEnableUI = true;
            HeistDividend::bEnable.store(true);
            
            // 设置佩里科岛分红为180%
            HeistDividend::cayoDividend1PUI = 180;
            HeistDividend::cayoDividend2PUI = 180;
            HeistDividend::cayoDividend3PUI = 180;
            HeistDividend::cayoDividend4PUI = 180;
            
            HeistDividend::cayoDividend1P.store(180);
            HeistDividend::cayoDividend2P.store(180);
            HeistDividend::cayoDividend3P.store(180);
            HeistDividend::cayoDividend4P.store(180);
            
            // 调用UpdateDividends写入游戏内存
            HeistDividend::UpdateDividends();
        }
        
        // 右侧：实时分红值显示（移动到预设按钮旁边）
        ImGui::NextColumn();
        
        // 实时分红值显示
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "实时分红值：");
        ImGui::Spacing();
        
        // 为实时分红值区域添加更多空间
        ImGui::Spacing();
        
        // 赌场豪劫
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "🎰 赌场豪劫：");
        ImGui::Text("1P: %d%%", realtimeDividends[0]);
        ImGui::Text("2P: %d%%", realtimeDividends[1]);
        ImGui::Text("3P: %d%%", realtimeDividends[2]);
        ImGui::Text("4P: %d%%", realtimeDividends[3]);
        
        // 增加间距
        ImGui::Spacing();
        ImGui::Spacing();
        
        // 佩里科岛
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "🏝️ 佩里科岛：");
        ImGui::Text("1P: %d%%", realtimeDividends[4]);
        ImGui::Text("2P: %d%%", realtimeDividends[5]);
        ImGui::Text("3P: %d%%", realtimeDividends[6]);
        ImGui::Text("4P: %d%%", realtimeDividends[7]);
        
        // 为后期扩展预留空间
        ImGui::Spacing();
        ImGui::Spacing();
        
        // 结束列布局
        ImGui::Columns(1);
        
        // 增加更大的间距
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Spacing();
        
        // 分红操作
        ImGui::TextColored(ImVec4(0.26f, 0.59f, 0.98f, 1.0f), "分红操作：");
        ImGui::Spacing();
        
        // 复制实时值到输入框按钮（自适应宽度）
        if (ImGui::Button("复制实时值到输入框", ImVec2(-1, 30))) {
            // 复制实时分红值到所有玩家的输入框
            // 赌场豪劫分红
            HeistDividend::casinoDividend1PUI = realtimeDividends[0];
            HeistDividend::casinoDividend2PUI = realtimeDividends[1];
            HeistDividend::casinoDividend3PUI = realtimeDividends[2];
            HeistDividend::casinoDividend4PUI = realtimeDividends[3];
            
            // 佩里科岛分红
            HeistDividend::cayoDividend1PUI = realtimeDividends[4];
            HeistDividend::cayoDividend2PUI = realtimeDividends[5];
            HeistDividend::cayoDividend3PUI = realtimeDividends[6];
            HeistDividend::cayoDividend4PUI = realtimeDividends[7];
            
            // 同步到原子变量
            HeistDividend::casinoDividend1P.store(realtimeDividends[0]);
            HeistDividend::casinoDividend2P.store(realtimeDividends[1]);
            HeistDividend::casinoDividend3P.store(realtimeDividends[2]);
            HeistDividend::casinoDividend4P.store(realtimeDividends[3]);
            HeistDividend::cayoDividend1P.store(realtimeDividends[4]);
            HeistDividend::cayoDividend2P.store(realtimeDividends[5]);
            HeistDividend::cayoDividend3P.store(realtimeDividends[6]);
            HeistDividend::cayoDividend4P.store(realtimeDividends[7]);
            
            std::println("已复制实时分红值到所有输入框");
        }
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        
        // 左右布局 - 赌场豪劫和佩里科岛分红并列
        ImGui::Columns(2, "dividend_layout", false);
        
        // 左侧：赌场豪劫分红（包含所有4个玩家）
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "🎰 赌场豪劫分红设置：");
        ImGui::Spacing();
        
        // 赌场1P - 当前值、输入框和应用按钮
        ImGui::Text("赌场1P当前分红: %d%%", HeistDividend::casinoDividend1PUI);
        int newCasinoDividend1P = HeistDividend::casinoDividend1PUI;
        if (ImGui::InputInt("##casino_p1_input", &newCasinoDividend1P, 0, 0, ImGuiInputTextFlags_CharsDecimal)) {
            // 限制输入范围 - 允许超过100
            newCasinoDividend1P = std::max(0, newCasinoDividend1P);
            HeistDividend::casinoDividend1PUI = newCasinoDividend1P;
            HeistDividend::casinoDividend1P.store(newCasinoDividend1P);
        }
        if (ImGui::Button("应用##casino_p1_apply", ImVec2(-1, 25))) {
            // 写入赌场1P分红值到游戏内存
            HeistDividend::WriteCasinoDividend(1, newCasinoDividend1P);
            std::println("已应用赌场1P分红值: {}%", newCasinoDividend1P);
        }
        
        ImGui::Spacing();
        
        // 赌场2P - 当前值、输入框和应用按钮
        ImGui::Text("赌场2P当前分红: %d%%", HeistDividend::casinoDividend2PUI);
        int newCasinoDividend2P = HeistDividend::casinoDividend2PUI;
        if (ImGui::InputInt("##casino_p2_input", &newCasinoDividend2P, 0, 0, ImGuiInputTextFlags_CharsDecimal)) {
            // 限制输入范围 - 允许超过100
            newCasinoDividend2P = std::max(0, newCasinoDividend2P);
            HeistDividend::casinoDividend2PUI = newCasinoDividend2P;
            HeistDividend::casinoDividend2P.store(newCasinoDividend2P);
        }
        if (ImGui::Button("应用##casino_p2_apply", ImVec2(-1, 25))) {
            // 写入赌场2P分红值到游戏内存
            HeistDividend::WriteCasinoDividend(2, newCasinoDividend2P);
            std::println("已应用赌场2P分红值: {}%", newCasinoDividend2P);
        }
        
        ImGui::Spacing();
        
        // 赌场3P - 当前值、输入框和应用按钮
        ImGui::Text("赌场3P当前分红: %d%%", HeistDividend::casinoDividend3PUI);
        int newCasinoDividend3P = HeistDividend::casinoDividend3PUI;
        if (ImGui::InputInt("##casino_p3_input", &newCasinoDividend3P, 0, 0, ImGuiInputTextFlags_CharsDecimal)) {
            // 限制输入范围 - 允许超过100
            newCasinoDividend3P = std::max(0, newCasinoDividend3P);
            HeistDividend::casinoDividend3PUI = newCasinoDividend3P;
            HeistDividend::casinoDividend3P.store(newCasinoDividend3P);
        }
        if (ImGui::Button("应用##casino_p3_apply", ImVec2(-1, 25))) {
            // 写入赌场3P分红值到游戏内存
            HeistDividend::WriteCasinoDividend(3, newCasinoDividend3P);
            std::println("已应用赌场3P分红值: {}%", newCasinoDividend3P);
        }
        
        ImGui::Spacing();
        
        // 赌场4P - 当前值、输入框和应用按钮
        ImGui::Text("赌场4P当前分红: %d%%", HeistDividend::casinoDividend4PUI);
        int newCasinoDividend4P = HeistDividend::casinoDividend4PUI;
        if (ImGui::InputInt("##casino_p4_input", &newCasinoDividend4P, 0, 0, ImGuiInputTextFlags_CharsDecimal)) {
            // 限制输入范围 - 允许超过100
            newCasinoDividend4P = std::max(0, newCasinoDividend4P);
            HeistDividend::casinoDividend4PUI = newCasinoDividend4P;
            HeistDividend::casinoDividend4P.store(newCasinoDividend4P);
        }
        if (ImGui::Button("应用##casino_p4_apply", ImVec2(-1, 25))) {
            // 写入赌场4P分红值到游戏内存
            HeistDividend::WriteCasinoDividend(4, newCasinoDividend4P);
            std::println("已应用赌场4P分红值: {}%", newCasinoDividend4P);
        }
        
        ImGui::Spacing();
        
        // 计算赌场豪劫分红总和 (不再强制要求)
        int casinoTotal = HeistDividend::casinoDividend1PUI + HeistDividend::casinoDividend2PUI + 
                         HeistDividend::casinoDividend3PUI + HeistDividend::casinoDividend4PUI;
        
        if (casinoTotal == 100) {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "赌场豪劫分红总和: %d%% (正确)##casino_total", casinoTotal);
        } else {
            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "赌场豪劫分红总和: %d%% (注意: 不为100%%)##casino_total", casinoTotal);
            if (casinoTotal > 100) {
                ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "⚠️ 警告: 超出100%%是黑钱##casino_warning");
            }
        }
        
        // 右侧：佩里科岛分红（包含所有4个玩家）
        ImGui::NextColumn();
        
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "🏝️ 佩里科岛分红设置：");
        ImGui::Spacing();
        
        // 佩岛1P - 当前值、输入框和应用按钮
        ImGui::Text("佩岛1P当前分红: %d%%", HeistDividend::cayoDividend1PUI);
        int newCayoDividend1P = HeistDividend::cayoDividend1PUI;
        if (ImGui::InputInt("##cayo_p1_input", &newCayoDividend1P, 0, 0, ImGuiInputTextFlags_CharsDecimal)) {
            // 限制输入范围 - 允许超过100
            newCayoDividend1P = std::max(0, newCayoDividend1P);
            HeistDividend::cayoDividend1PUI = newCayoDividend1P;
            HeistDividend::cayoDividend1P.store(newCayoDividend1P);
        }
        if (ImGui::Button("应用##cayo_p1_apply", ImVec2(-1, 25))) {
            // 写入佩岛1P分红值到游戏内存
            HeistDividend::WriteCayoDividend(1, newCayoDividend1P);
            std::println("已应用佩岛1P分红值: {}%", newCayoDividend1P);
        }
        
        ImGui::Spacing();
        
        // 佩岛2P - 当前值、输入框和应用按钮
        ImGui::Text("佩岛2P当前分红: %d%%", HeistDividend::cayoDividend2PUI);
        int newCayoDividend2P = HeistDividend::cayoDividend2PUI;
        if (ImGui::InputInt("##cayo_p2_input", &newCayoDividend2P, 0, 0, ImGuiInputTextFlags_CharsDecimal)) {
            // 限制输入范围 - 允许超过100
            newCayoDividend2P = std::max(0, newCayoDividend2P);
            HeistDividend::cayoDividend2PUI = newCayoDividend2P;
            HeistDividend::cayoDividend2P.store(newCayoDividend2P);
        }
        if (ImGui::Button("应用##cayo_p2_apply", ImVec2(-1, 25))) {
            // 写入佩岛2P分红值到游戏内存
            HeistDividend::WriteCayoDividend(2, newCayoDividend2P);
            std::println("已应用佩岛2P分红值: {}%", newCayoDividend2P);
        }
        
        ImGui::Spacing();
        
        // 佩岛3P - 当前值、输入框和应用按钮
        ImGui::Text("佩岛3P当前分红: %d%%", HeistDividend::cayoDividend3PUI);
        int newCayoDividend3P = HeistDividend::cayoDividend3PUI;
        if (ImGui::InputInt("##cayo_p3_input", &newCayoDividend3P, 0, 0, ImGuiInputTextFlags_CharsDecimal)) {
            // 限制输入范围 - 允许超过100
            newCayoDividend3P = std::max(0, newCayoDividend3P);
            HeistDividend::cayoDividend3PUI = newCayoDividend3P;
            HeistDividend::cayoDividend3P.store(newCayoDividend3P);
        }
        if (ImGui::Button("应用##cayo_p3_apply", ImVec2(-1, 25))) {
            // 写入佩岛3P分红值到游戏内存
            HeistDividend::WriteCayoDividend(3, newCayoDividend3P);
            std::println("已应用佩岛3P分红值: {}%", newCayoDividend3P);
        }
        
        ImGui::Spacing();
        
        // 佩岛4P - 当前值、输入框和应用按钮
        ImGui::Text("佩岛4P当前分红: %d%%", HeistDividend::cayoDividend4PUI);
        int newCayoDividend4P = HeistDividend::cayoDividend4PUI;
        if (ImGui::InputInt("##cayo_p4_input", &newCayoDividend4P, 0, 0, ImGuiInputTextFlags_CharsDecimal)) {
            // 限制输入范围 - 允许超过100
            newCayoDividend4P = std::max(0, newCayoDividend4P);
            HeistDividend::cayoDividend4PUI = newCayoDividend4P;
            HeistDividend::cayoDividend4P.store(newCayoDividend4P);
        }
        if (ImGui::Button("应用##cayo_p4_apply", ImVec2(-1, 25))) {
            // 写入佩岛4P分红值到游戏内存
            HeistDividend::WriteCayoDividend(4, newCayoDividend4P);
            std::println("已应用佩岛4P分红值: {}%", newCayoDividend4P);
        }
        
        ImGui::Spacing();
        
        // 计算佩里科岛分红总和 (不再强制要求)
        int cayoTotal = HeistDividend::cayoDividend1PUI + HeistDividend::cayoDividend2PUI + 
                       HeistDividend::cayoDividend3PUI + HeistDividend::cayoDividend4PUI;
        
        if (cayoTotal == 100) {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "佩里科岛分红总和: %d%% (正确)##cayo_total", cayoTotal);
        } else {
            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "佩里科岛分红总和: %d%% (注意: 不为100%%)##cayo_total", cayoTotal);
            if (cayoTotal > 100) {
                ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "⚠️ 警告: 超出100%%是黑钱##cayo_warning");
            }
        }
        
        // 结束列布局
        ImGui::Columns(1);
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
    } else {
        // 功能未启用时的提示
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "请启用抢劫分红修改功能以使用分红设置##disabled_note");
    }
}

void MenuManager::RenderHeistDividendPage()
{
    // 设置窗口大小
    SetupPageWindow(ImVec2(550, 650));
    
    // 开始窗口
    if (!ImGui::Begin("抢劫任务分红", nullptr, ImGuiWindowFlags_NoCollapse)) {
        ImGui::End();
        return;
    }
    
    RenderHeistDividendPageContent();
    
    // 返回按钮
    RenderBackButton();
    
    // 结束窗口
    ImGui::End();
}







void MenuManager::RenderSettingsPageContent() {
    // 标题
    RenderPageTitle("设置");
    
    ImGui::Separator();
    
    // 主题设置
    ImGui::TextColored(ImVec4(0.26f, 0.59f, 0.98f, 1.0f), "主题设置");
    ImGui::Spacing();
    
    // 主题选择器
    ImGuiToolStyle::RenderThemeSelector();
    
    ImGui::Separator();
    
    // 高级功能
    ImGui::TextColored(ImVec4(0.26f, 0.59f, 0.98f, 1.0f), "高级功能");
    ImGui::Spacing();
    
    // 追战局功能
    ImGui::Checkbox("启用追战局", &PlayerChaser::bEnableUI);
}

void MenuManager::RenderSettingsPage()
{
    SetupPageWindow();
    ImGui::Begin("设置", nullptr, ImGuiWindowFlags_NoCollapse);
    RenderSettingsPageContent();
    RenderBackButton();
    ImGui::End();
}

void MenuManager::RenderBackButton()
{
    ImGui::Separator();
    
    // 返回按钮
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.60f, 0.20f, 0.20f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.70f, 0.30f, 0.30f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.50f, 0.10f, 0.10f, 1.0f));
    
    if (ImGui::Button("返回", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
        GoBack();
    }
    
    ImGui::PopStyleColor(3);
}

bool MenuManager::RenderMenuItem(const std::string& label, MenuPage targetPage, bool enabled)
{
    if (!enabled) {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
    }
    
    bool clicked = ImGui::Button(label.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0));
    
    if (!enabled) {
        ImGui::PopStyleVar();
    } else if (clicked) {
        SwitchToPage(targetPage);
    }
    
    return clicked;
}

void MenuManager::RenderPageTitle(const std::string& title)
{
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));
    ImGui::Text(title.c_str());
    ImGui::PopStyleColor();
}

void MenuManager::SetupPageWindow(const ImVec2& size)
{
    ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
}