#ifndef SUPERMONKEY_HPP
#define SUPERMONKEY_HPP
#include "Util/Logger.hpp"
#include "Util/Time.hpp"
#include "components/canBuy.hpp"
#include "components/collisionComp.hpp"
#include "conf.hpp"
#include "entities/poppers/dart.hpp"
#include "tower.hpp"

class SuperMonkey final : public Tower::Tower {
private:
  int m_cooldown = 5;        // 射擊冷卻時間（幀數）- 比 DartMonkey 更快
  int m_currentCooldown = 0; // 當前剩餘冷卻時間
  bool m_draggable = false;  // 是否可拖曳
  bool m_clickable = true;   // 是否可點擊
  Components::CollisionComponent m_collision;

public:
  SuperMonkey(const Util::PTSDPosition &position,
              float range = float(RANGE_SUPER));
  ~SuperMonkey() = default;

  // 處理射程內氣球的方法
  void handleBloonsInRange(const std::vector<std::shared_ptr<Bloon>> &bloons,
                           const std::vector<float> &distances) override;

  // I_draggable 介面實現
  void onDragStart() override { LOG_DEBUG("SPMky : Dragging start"); };
  void onDrag(const Util::PTSDPosition &newPosition) override {
    m_body->setPosition(newPosition);
    m_range->setPosition(newPosition);
    m_collision.setPosition(newPosition);
  };
  void onDragEnd() override { LOG_DEBUG("SPMky : Dragging end"); };
  bool isDraggable() const override { return m_draggable; }
  void setDraggable(bool draggable) override { m_draggable = draggable; }

  // 升級系統
  void setFirstUpgrade(int cost) override {
    m_info.firstUpgrade = true;
    m_info.investmentCost += cost;
    m_info.attackRange += 100.0f;             // 增加攻擊範圍
    m_range->setRadius(RANGE_SUPER + 100.0f); // 更新實際射程圓圈
  }

  void setSecondUpgrade(int cost) override {
    m_info.secondUpgrade = true;
    m_info.investmentCost += cost;
    // Laser upgrade - 增加穿透力和特殊能力（跟 DartMonkey 的 piercing dart
    // 一樣）
  }

  // CollisionComponent 實現
  std::shared_ptr<Components::CollisionComponent>
  getCollisionComponent() const override {
    return std::make_shared<Components::CollisionComponent>(m_collision);
  }

public:
  SuperMonkey(const SuperMonkey &) = default;
  SuperMonkey(SuperMonkey &&) = delete;
  SuperMonkey &operator=(const SuperMonkey &) = default;
  SuperMonkey &operator=(SuperMonkey &&) = delete;
};
#endif
