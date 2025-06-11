#ifndef DART_HPP
#define DART_HPP
#include "interfaces/move.hpp"
#include "popper.hpp"
#include "components/collisionComp.hpp"
#include <glm/glm.hpp>

class Dart : public popper, public Interface::I_move {
private:
  std::shared_ptr<Util::GameObject> m_object;
  std::shared_ptr<Components::CollisionComponent> m_collisionComponent; // 添加碰撞組件
  int life;
  float speed = 10.0f;
  glm::vec2 m_direction;
  bool is_outside_bounds = false;

public:
  Dart(const Util::PTSDPosition &start, const Util::PTSDPosition &end, int life=1);
  
  // 添加 setPosition 方法
  void setPosition(const Util::PTSDPosition &position) override;
  Util::PTSDPosition get_position() const override;

  // 從 I_move 繼承的方法
  void move() override;
  Util::PTSDPosition next_position(int frames = 1) override;
  
  // 從 popper 繼承的方法
  std::vector<bool> hit(std::vector<std::shared_ptr<Bloon>> bloons) override;
  std::shared_ptr<Util::GameObject> get_object() override;
};

#endif // DART_HPP