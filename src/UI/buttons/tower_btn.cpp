#include "UI/buttons/tower_btn.hpp"
#include "Util/Logger.hpp"

TowerButton::TowerButton(const std::string &name, 
                         const Util::PTSDPosition &pos,
                         std::variant<glm::vec2, float> col_parm, 
                         bool can_click,
                         Tower::TowerType towerType,
                         int cost)
    : Button(name, pos, col_parm, can_click),
      m_towerType(towerType),
      m_cost(cost) {
    
    LOG_DEBUG("TOWER_BTN: 創建了一個 {} 塔按鈕，類型：{}, 成本：{}", 
              name, static_cast<int>(towerType), cost);
}

Tower::TowerType TowerButton::getTowerType() const {
    return m_towerType;
}

int TowerButton::getCost() const {
    return m_cost;
}

void TowerButton::onClick(){
    Button::onClick();
    
    LOG_DEBUG("TOWER_BTN: 玩家點擊了 {} 塔按鈕，類型：{}", 
              getName(), static_cast<int>(m_towerType));
}