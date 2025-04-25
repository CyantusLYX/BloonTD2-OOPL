#ifndef UI_TOWER_BUTTONS_HPP
#define UI_TOWER_BUTTONS_HPP

#include "UI/container/container.hpp"
#include "UI/buttons/tower_btn.hpp"
#include <memory>
#include <vector>

namespace UI {

// 塔購買按鈕面板，自動管理並排列TowerButton
class TowerButtonsPanel : public UIContainer {
private:
    // 每行最大按鈕數
    size_t m_buttonsPerRow = 4;
    
    // 按鈕大小
    glm::vec2 m_buttonSize = {60.0f, 60.0f};
    
    // 當前的按鈕行
    std::vector<std::shared_ptr<UIContainer>> m_rows;

    // 創建新的按鈕行
    std::shared_ptr<UIContainer> createRow();
    
    // 重新排列所有按鈕
    void rearrangeButtons();

public:
    // 構造函數
    TowerButtonsPanel(
        const Util::PTSDPosition &position,
        float width,
        float height,
        float zIndex = 10.0f
    );
    
    // 添加一個塔購買按鈕
    void addTowerButton(const std::shared_ptr<TowerButton> &button);
    
    // 設置每行按鈕數量
    void setButtonsPerRow(size_t count);
    
    // 設置按鈕大小
    void setButtonSize(const glm::vec2 &size);
    
    // 清除所有按鈕
    void clearButtons();
    
    // 獲取所有按鈕
    std::vector<std::shared_ptr<TowerButton>> getAllButtons() const;

    void updateLayout() override;
};

} // namespace UI

#endif // UI_TOWER_BUTTONS_HPP