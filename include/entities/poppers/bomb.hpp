#ifndef BOMB_HPP
#define BOMB_HPP
#include "components/collisionComp.hpp"
#include "interfaces/move.hpp"
#include "popper.hpp"
#include <glm/glm.hpp>

class Bomb : public popper, public Interface::I_move {
private:
  std::shared_ptr<Util::GameObject> m_object;
  std::shared_ptr<Components::CollisionComponent>
      m_triggerComponent; // 觸發範圍
  std::shared_ptr<Components::CollisionComponent>
      m_explosionComponent; // 爆炸範圍
  float speed = 15.0f;
  glm::vec2 m_direction;
  bool m_exploded = false;
  bool m_isMoving = true;
  float m_maxDistance;
  float m_movedDistance = 0.0f;

public:
  // 建構子接受爆炸範圍作為參數
  Bomb(const Util::PTSDPosition &start, float angle,
       float triggerRadius = 40.0f, float explosionRadius = 60.0f,
       float maxDistance = 300.0f);

  // 從 CollisionComponent 繼承的方法
  void setPosition(const Util::PTSDPosition &position) override;
  Util::PTSDPosition get_position() const override;

  // 從 I_move 繼承的方法
  void move() override;
  Util::PTSDPosition next_position(int frames = 1) override;

  // 從 popper 繼承的方法
  std::vector<bool> hit(std::vector<std::shared_ptr<Bloon>> bloons) override;
  std::shared_ptr<Util::GameObject> get_object() override;

  // 爆炸相關方法
  bool isExploded() const { return m_exploded; }
  void explode();

  // 檢查氣球是否在觸發範圍內
  bool isInTriggerRange(const std::shared_ptr<Bloon> &bloon) const;

  // 提供爆炸碰撞組件給 Manager 使用
  std::shared_ptr<Components::CollisionComponent>
  getExplosionComponent() const {
    return m_explosionComponent;
  }
};

#endif // BOMB_HPP
