#ifndef BTN_TOWER_HPP
#define BTN_TOWER_HPP

#include "UI/button.hpp"
#include "entities/tower/tower.hpp"
#include "components/canBuy.hpp"
#include <memory>

class TowerButton : public Button {
private:
    Tower::TowerType m_towerType;
    int m_cost;

public:
    // 構造函數
    TowerButton(const std::string &name, 
                const Util::PTSDPosition &pos,
                std::variant<glm::vec2, float> col_parm, 
                const std::string &path,
                bool can_click,
                Tower::TowerType towerType,
                int cost);
    
    // 獲取塔類型
    Tower::TowerType getTowerType() const;
    
    // 獲取塔成本
    int getCost() const;
    
    void onClick() override;
};

#endif // BTN_TOWER_HPP