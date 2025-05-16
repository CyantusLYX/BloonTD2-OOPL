#include "core/manager.hpp"
#include "entities/tower/all_tower.hpp"
// 初始化塔工廠映射表
void Manager::initTowerFactories() {
  // 註冊 DartMonkey 工廠函數
  m_towerFactories[Tower::TowerType::dart] = [](const Util::PTSDPosition &pos) {
    return std::make_shared<DartMonkey>(pos);
  };
  m_towerFactories[Tower::TowerType::ice] = [](const Util::PTSDPosition &pos) {
    return std::make_shared<IceBall>(pos);
  };
  m_towerFactories[Tower::TowerType::tack] = [](const Util::PTSDPosition &pos) {
    return std::make_shared<TackShooter>(pos);
  };
  m_towerFactories[Tower::TowerType::bomb] = [](const Util::PTSDPosition &pos) {
    return std::make_shared<Cannon>(pos);
  };
  m_towerFactories[Tower::TowerType::boomerang] = [](const Util::PTSDPosition &pos) {
    return std::make_shared<BoomerangMonkey>(pos);
  };

  // 初始化 popper 工廠映射表
  m_popperFactories[Tower::TowerType::spike] =
      [](const Util::PTSDPosition &pos) {
        return std::make_shared<spike>(pos);
      };

  // 可以繼續添加其他 popper 類型...
}