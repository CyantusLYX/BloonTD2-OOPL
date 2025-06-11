#ifndef TACK_SHOOTER_HPP
#define TACK_SHOOTER_HPP

#include "Util/Logger.hpp"
#include "components/collisionComp.hpp"
#include "conf.hpp"
#include "config.hpp"
#include "entities/bloon.hpp"
#include "entities/tower/tower.hpp"
#include <memory>

class TackShooter final : public Tower::Tower {
private:
  int m_cooldown = 90;       // 射擊冷卻時間（幀數）
  int m_currentCooldown = 0; // 當前剩餘冷卻時間
  bool m_draggable = false;  // 是否可拖曳
  bool m_clickable = true;   // 是否可點擊
  Components::CollisionComponent m_collision;

public:
  TackShooter(const Util::PTSDPosition &position, float range = RANGE_TACK);
  ~TackShooter() = default;

  // 處理射程內氣球的方法 - 發射 8 個方向的 tack
  void handleBloonsInRange(const std::vector<std::shared_ptr<Bloon>> &bloons,
                           const std::vector<float> &distances) override;

  // I_draggable 介面實現
  void onDragStart() override { LOG_DEBUG("TACKS : Dragging start"); }
  void onDrag(const Util::PTSDPosition &newPosition) override {
    m_body->setPosition(newPosition);
    m_range->setPosition(newPosition);
    m_collision.setPosition(newPosition);
  }
  void onDragEnd() override { LOG_DEBUG("TACKS : Dragging end"); }
  bool isDraggable() const override { return m_draggable; }
  void setDraggable(bool draggable) override { m_draggable = draggable; }

  // 升級系統實現
  void setFirstUpgrade(int cost) override {
    m_info.firstUpgrade = true;
    m_info.investmentCost += cost;
    m_cooldown = 60;
  }

  void setSecondUpgrade(int cost) override {
    m_info.secondUpgrade = true;
    m_info.investmentCost += cost;
    float newRange = RANGE_TACK + 10.0f;
    m_info.attackRange = newRange; // 更新資訊中的攻擊範圍
    m_range->setRadius(newRange);  // 增加射程
    // 第二個升級還會增加 tack 的射程距離，在 handleBloonsInRange 中處理
  }

  // CollisionComponent 的實現
  std::shared_ptr<Components::CollisionComponent>
  getCollisionComponent() const override {
    return m_range->getCollisionComponent();
  }
};

#endif // TACK_SHOOTER_HPP
