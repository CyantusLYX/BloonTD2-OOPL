
#include "core/manager.hpp"
#include "entities/tower/ice_ball.hpp"
// 初始化塔工廠映射表
void Manager::initTowerFactories() {
  // 註冊 DartMonkey 工廠函數
  m_towerFactories[Tower::TowerType::dart] = [](const Util::PTSDPosition &pos) {
    return std::make_shared<DartMonkey>(pos);
  };
  m_towerFactories[Tower::TowerType::ice] = [](const Util::PTSDPosition &pos) {
    return std::make_shared<IceBall>(pos);
  };

  // 註冊 BoomerangMonkey 工廠函數（如果已實現）
  /* m_towerFactories[Tower::TowerType::boomerang] = [](const Util::PTSDPosition
  &pos) {
      // 如果未實現，返回 nullptr 或拋出異常
      LOG_ERROR("MNGR  : BoomerangMonkey 尚未實現");
      return nullptr;
      // 或者 return std::make_shared<BoomerangMonkey>(pos);
  }; */

  // 可以繼續添加其他塔類型...

  // 初始化 popper 工廠映射表
  m_popperFactories[Tower::TowerType::spike] =
      [](const Util::PTSDPosition &pos) {
        return std::make_shared<spike>(pos);
      };

  // 可以繼續添加其他 popper 類型...
}