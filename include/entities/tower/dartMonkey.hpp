#ifndef DARTMONKEY_HPP
#define DARTMONKEY_HPP
#include "Util/Logger.hpp"
#include "Util/Time.hpp"
#include "components/canBuy.hpp"
#include "components/collisionComp.hpp"
#include "conf.hpp"
#include "entities/poppers/dart.hpp"
#include "tower.hpp"

class DartMonkey final : public Tower::Tower {
private:
  int m_cooldown = 25;       // 射擊冷卻時間（幀數）
  int m_currentCooldown = 0; // 當前剩餘冷卻時間
  bool m_draggable = false;  // 是否可拖曳
  bool m_clickable = true;   // 是否可點擊
  Components::CollisionComponent m_collision;

public:
  DartMonkey(const Util::PTSDPosition &position,
             float range = float(RANGE_DART));
  ~DartMonkey() = default;

  // 處理射程內氣球的方法
  void handleBloonsInRange(const std::vector<std::shared_ptr<Bloon>> &bloons,
                           const std::vector<float> &distances) override;

  // I_draggable 介面實現
  void onDragStart() override { LOG_DEBUG("DTMky : Dragging start"); };
  void onDrag(const Util::PTSDPosition &newPosition) override {
    m_body->setPosition(newPosition);
    m_range->setPosition(newPosition);
    m_collision.setPosition(newPosition);
  };
  void onDragEnd() override { LOG_DEBUG("DTMky : Dragging end"); };
  bool isDraggable() const override { return m_draggable; }
  void setDraggable(bool draggable) override { m_draggable = draggable; }
  void setFirstUpgrade(int cost) override {
    m_info.firstUpgrade = true;
    m_info.investmentCost += cost;
    m_info.attackRange += 25.0f;            // 增加攻擊範圍
    m_range->setRadius(RANGE_DART + 25.0f); // 增加射程
  }
  // components::collisionComp

public:
  DartMonkey(const DartMonkey &) = default;
  DartMonkey(DartMonkey &&) = delete;
  DartMonkey &operator=(const DartMonkey &) = default;
  DartMonkey &operator=(DartMonkey &&) = delete;
};
#endif
