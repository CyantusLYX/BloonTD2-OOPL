#ifndef COMP_CANBUY_HPP
#define COMP_CANBUY_HPP
#include "components/towerType.hpp"
#include "interfaces/draggable.hpp"
class CanBuy : public Interface::I_draggable {
private:
  Tower::TowerType m_towerType;
  int m_cost;

public:
  CanBuy(Tower::TowerType towerType, int cost)
      : m_towerType(towerType),
        m_cost(cost) {};
  CanBuy() = default;
  ~CanBuy() override = default;
  Tower::TowerType getTowerType() const { return m_towerType; }
  int getCost() const { return m_cost; }
};
#endif
