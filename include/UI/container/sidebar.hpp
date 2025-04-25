#ifndef UI_SIDEBAR_HPP
#define UI_SIDEBAR_HPP

#include "UI/container/container.hpp"
#include "UI/container/status.hpp"
#include "UI/container/towerBtns.hpp"
#include <memory>
#include <string>

namespace UI {

// 側邊欄的主類別
class UISidebar : public UIContainer {
private:
    // 使用專用元件取代簡單容器
    std::shared_ptr<StatusBar> m_statusBar;             // 狀態列元件
    std::shared_ptr<TowerButtonsPanel> m_towerBtnPanel; // 塔按鈕面板元件
    std::shared_ptr<UIContainer> m_towerInfoSection;    // 塔資訊/升級區域
    std::shared_ptr<UIContainer> m_controlSection;      // 控制按鈕區域
    
public:
    // 建構函式
    UISidebar(
        const Util::PTSDPosition &position, 
        float height,
        float width = 150.0f,
        float zIndex = 10.0f
    );
    
    // 狀態更新函式 (委託給 StatusBar)
    void updateMoney(int amount);
    void updateLives(int amount);
    
    // 塔按鈕操作 (委託給 TowerButtonsPanel)
    void addTowerButton(const std::shared_ptr<TowerButton> &button);
    std::vector<std::shared_ptr<TowerButton>> getAllTowerButtons() const;
    void clearTowerButtons();
    
    // 獲取各區域元件
    std::shared_ptr<StatusBar> getStatusBar() const { return m_statusBar; }
    std::shared_ptr<TowerButtonsPanel> getTowerButtonsPanel() const { return m_towerBtnPanel; }
    std::shared_ptr<UIContainer> getTowerInfoSection() const { return m_towerInfoSection; }
    std::shared_ptr<UIContainer> getControlSection() const { return m_controlSection; }
    
    // 清除所有區域內容
    void clearSections();
    
    // 更新佈局 (覆蓋基類方法)
    void updateLayout() override;
};

} // namespace UI

#endif // UI_SIDEBAR_HPP