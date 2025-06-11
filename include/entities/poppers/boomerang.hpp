#ifndef BOOMERANG_HPP
#define BOOMERANG_HPP
#include "components/collisionComp.hpp"
#include "interfaces/move.hpp"
#include "popper.hpp"
#include <cmath>
#include <glm/glm.hpp>
#include <memory>

class Boomerang : public popper, public Interface::I_move {
private:
  std::shared_ptr<Util::GameObject> m_object;
  std::shared_ptr<Components::CollisionComponent> m_collisionComponent;

  Util::PTSDPosition m_startPos; // 起始位置 (塔的位置)
  float m_angle;                 // 初始角度
  float m_diameter;              // 圓的直徑
  float m_currentAngle;          // 當前在圓上的角度
  float m_angularSpeed;          // 角速度 (弧度/幀)
  int m_life;                    // 可以擊中的次數

  bool m_hasCompletedPath; // 是否已完成整個路徑

public:
  // 構造函數接收起始位置、角度、直徑和生命值
  Boomerang(const Util::PTSDPosition &start, float angle, float diameter,
            int life = 3);
  ~Boomerang() = default;

  // 從 popper 繼承的方法
  std::vector<bool> hit(std::vector<std::shared_ptr<Bloon>> bloons) override;
  std::shared_ptr<Util::GameObject> get_object() override;

  // 從 I_move 繼承的方法
  void move() override;
  Util::PTSDPosition next_position(int frames = 1) override;

  // 從 CollisionComponent 繼承的方法
  void setPosition(const Util::PTSDPosition &position) override;
  Util::PTSDPosition get_position() const override;
};

#endif // BOOMERANG_HPP
