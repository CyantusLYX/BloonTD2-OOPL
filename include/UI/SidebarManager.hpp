#ifndef UI_SIDEBAR_MANAGER_HPP
#define UI_SIDEBAR_MANAGER_HPP

#include "Util/GameObject.hpp"
#include "Util/Position.hpp"
#include "UI/container/status.hpp"
#include "UI/container/towerBtns.hpp"
#include "UI/Flag.hpp"
#include "Util/Renderer.hpp"
#include "core/shape.hpp"
#include <memory>
#include <vector>

namespace UI {

class SidebarManager {
private:
    // 背景對象和可繪製元素
    std::shared_ptr<Util::GameObject> m_background;
    std::shared_ptr<Util::Shape> m_backgroundShape;
    
    // 各個功能區域
    std::shared_ptr<StatusBar> m_statusBar;
    std::shared_ptr<TowerButtonsPanel> m_towerBtnPanel; 
    std::shared_ptr<Flag> m_infoFlag;  // Flag基類引用
    
    // 全域參考
    std::weak_ptr<Util::Renderer> m_renderer; // 渲染器引用
    
    // 尺寸和位置
    Util::PTSDPosition m_position;
    glm::vec2 m_size;
    float m_zIndex;
    
    // 重新計算子面板位置
    void recalculatePanelPositions();
    
public:
    // 建構函式
    SidebarManager(
        const Util::PTSDPosition &position, 
        float height,
        float width = 150.0f,
        float zIndex = 10.0f
    );
    
    // 設置渲染器引用
    void setRenderer(const std::shared_ptr<Util::Renderer>& renderer);
    
    // 顯示/隱藏
    void show(bool visible = true);
    
    // 位置控制
    void setPosition(const Util::PTSDPosition &position);
    Util::PTSDPosition getPosition() const { return m_position; }
    
    // 尺寸控制
    void setSize(const glm::vec2 &size);
    glm::vec2 getSize() const { return m_size; }
    
    // 功能方法 - 狀態更新
    void updateMoney(int amount);
    void updateLives(int amount);
    
    // 功能方法 - 塔按鈕
    void addTowerButton(const std::shared_ptr<TowerButton> &button);
    std::vector<std::shared_ptr<TowerButton>> getAllTowerButtons() const;
    void clearTowerButtons();
    
    // 獲取組件引用
    std::shared_ptr<StatusBar> getStatusBar() const { return m_statusBar; }
    std::shared_ptr<TowerButtonsPanel> getTowerButtonsPanel() const { return m_towerBtnPanel; }
    std::shared_ptr<Flag> getInfoFlag() const { return m_infoFlag; }
};

} // namespace UI

#endif // UI_SIDEBAR_MANAGER_HPP