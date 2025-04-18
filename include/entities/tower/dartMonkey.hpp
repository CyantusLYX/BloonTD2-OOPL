#ifndef DARTMONKEY_HPP
#define DARTMONKEY_HPP
#include "Util/Logger.hpp"
#include "Util/Time.hpp"
#include "components/collisionComp.hpp"
#include "entities/poppers/dart.hpp"
#include "interfaces/draggable.hpp"
#include "tower.hpp"

class DartMonkey final : public Tower::Tower, public Interface::I_draggable{
private:
  int m_cooldown = 25;       // 射擊冷卻時間（幀數）
  int m_currentCooldown = 0; // 當前剩餘冷卻時間
  bool m_draggable = false;  // 是否可拖曳
  bool m_clickable = true;  // 是否可點擊
  Components::CollisionComponent m_collision;

public:
  DartMonkey(const Util::PTSDPosition &position, float range = 150.0f);
  ~DartMonkey() = default;
  
  // 處理射程內氣球的方法
  void handleBloonsInRange(const std::vector<std::shared_ptr<Bloon>> &bloons,
                           const std::vector<float> &distances) override;

  // I_draggable 介面實現
  void onDragStart() override {
    LOG_DEBUG("DTMky : Dragging start");
  };
  void onDrag(const Util::PTSDPosition &newPosition) override{
    LOG_DEBUG("DTMky : Dragging");
    m_body->setPosition(newPosition);
    m_range->setPosition(newPosition);
    m_collision.setPosition(newPosition);
  };
  void onDragEnd() override {
    LOG_DEBUG("DTMky : Dragging end");
  };
  bool isDraggable() const override { return m_draggable; }
  void setDraggable(bool draggable) override { m_draggable = draggable; }

  //components::collisionComp

public:
  DartMonkey(const DartMonkey &) = default;
  DartMonkey(DartMonkey &&) = delete;
  DartMonkey &operator=(const DartMonkey &) = default;
  DartMonkey &operator=(DartMonkey &&) = delete;
};
#endif