#ifndef TACK_HPP
#define TACK_HPP
#include "components/collisionComp.hpp"
#include "interfaces/move.hpp"
#include "popper.hpp"
#include <glm/glm.hpp>

class Tack : public popper, public Interface::I_move {
private:
  std::shared_ptr<Util::GameObject> m_object;
  std::shared_ptr<Components::CollisionComponent> m_collisionComponent;
  float speed = 8.0f;
  glm::vec2 m_direction;
  bool is_outside_bounds = false;
  float m_maxDistance;   // 最大移動距離
  float m_movedDistance; // 已移動距離

public:
  // 構造函數，接收起始位置和角度
  Tack(const Util::PTSDPosition &start, float angle,
       float maxDistance = 200.0f);

  // 從 CollisionComponent 繼承的方法
  void setPosition(const Util::PTSDPosition &position) override;
  Util::PTSDPosition get_position() const override;

  // 從 I_move 繼承的方法
  void move() override;
  Util::PTSDPosition next_position(int frames = 1) override;

  // 從 popper 繼承的方法
  std::vector<bool> hit(std::vector<std::shared_ptr<Bloon>> bloons) override;
  std::shared_ptr<Util::GameObject> get_object() override;
};

#endif // TACK_HPP
