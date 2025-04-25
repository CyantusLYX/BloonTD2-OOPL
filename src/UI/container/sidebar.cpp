#include "UI/container/sidebar.hpp"
#include "Util/Logger.hpp"
#include "core/shape.hpp"

namespace UI {

UISidebar::UISidebar(
    const Util::PTSDPosition &position,
    float height,
    float width,
    float zIndex
) : UIContainer(position, glm::vec2(width, height), zIndex) {
    
    // 設置側邊欄外觀
    auto shape = std::dynamic_pointer_cast<Util::Shape>(m_Drawable);
    if (shape) {
        shape->SetColorRGB(180, 180, 180, 220); // 半透明灰色背景
    }
    m_Visible=true;
    // 設置較大的內邊距，美觀用
    setPadding(10.0f);
    setSpacing(5.0f);
    
    // 設置垂直佈局，從上到下排列元件
    setLayoutDirection(LayoutDirection::Vertical);
    setAlignment(Alignment::Start); // 上對齊
    
    // 計算各區域高度比例
    float statusHeight = height * 0.12f;  // 12% 給狀態區
    float buyHeight = height * 0.38f;     // 38% 給購買區
    float infoHeight = height * 0.35f;    // 35% 給塔資訊區
    float controlHeight = height * 0.15f; // 15% 給控制區
    
    // 創建狀態列元件
    m_statusBar = std::make_shared<StatusBar>(
        Util::PTSDPosition(0, 0), // 位置將由佈局設置
        width - 2*getPadding(),   // 寬度減去內邊距
        statusHeight,             // 高度
        zIndex + 0.1f,            // Z-index略高於父容器
        true                      // 固定大小
    );
    
    // 創建塔按鈕面板元件
    m_towerBtnPanel = std::make_shared<TowerButtonsPanel>(
        Util::PTSDPosition(0, 0), // 位置將由佈局設置
        width - 2*getPadding(),   // 寬度減去內邊距
        buyHeight,                // 高度
        zIndex + 0.1f             // Z-index略高於父容器
    );
    
    // 創建塔資訊區域容器
    m_towerInfoSection = std::make_shared<UIContainer>(
        Util::PTSDPosition(0, 0), // 位置將由佈局設置
        glm::vec2(width - 2*getPadding(), infoHeight),
        zIndex + 0.1f
    );
    m_towerInfoSection->setPadding(5.0f);
    m_towerInfoSection->SetVisible(false); // 預設隱藏
    
    // 創建控制區域容器
    m_controlSection = std::make_shared<UIContainer>(
        Util::PTSDPosition(0, 0), // 位置將由佈局設置
        glm::vec2(width - 2*getPadding(), controlHeight),
        zIndex + 0.1f
    );
    m_controlSection->setPadding(5.0f);
    
    // 添加所有區域到側邊欄
    addChild(m_statusBar);
    addChild(m_towerBtnPanel);
    addChild(m_towerInfoSection);
    addChild(m_controlSection);
    
    // 更新佈局
    updateLayout();
    
    LOG_DEBUG("UISidebar: Created with dimensions {}x{}", width, height);
}

void UISidebar::updateMoney(int amount) {
    if (m_statusBar) {
        m_statusBar->updateMoney(amount);
    }
}

void UISidebar::updateLives(int amount) {
    if (m_statusBar) {
        m_statusBar->updateLives(amount);
    }
}

void UISidebar::addTowerButton(const std::shared_ptr<TowerButton> &button) {
    if (m_towerBtnPanel) {
        m_towerBtnPanel->addTowerButton(button);
    }
}

void UISidebar::clearTowerButtons() {
    if (m_towerBtnPanel) {
        m_towerBtnPanel->clearButtons();
    }
}

void UISidebar::clearSections() {
    // 重置狀態列的值
    if (m_statusBar) {
        m_statusBar->updateMoney(0);
        m_statusBar->updateLives(0);
    }
    
    // 清除塔按鈕
    if (m_towerBtnPanel) {
        m_towerBtnPanel->clearButtons();
    }
    
    // 清除塔資訊區和控制區
    if (m_towerInfoSection) {
        m_towerInfoSection->clearChildren();
    }
    
    if (m_controlSection) {
        m_controlSection->clearChildren();
    }
}

std::vector<std::shared_ptr<TowerButton>> UISidebar::getAllTowerButtons() const {
    if (m_towerBtnPanel) {
        return m_towerBtnPanel->getAllButtons();
    }
    return {};  // 如果面板不存在則返回空vector
}

void UISidebar::updateLayout() {
    // 調用基類的 updateLayout()，處理子元素的排列
    UIContainer::updateLayout();
}

} // namespace UI