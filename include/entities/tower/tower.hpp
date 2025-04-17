#ifndef TOWER_HPP
#define TOWER_HPP
#include "Util/GameObject.hpp"
#include "Util/Position.hpp"
#include "components/collisionComp.hpp"
#include "core/shape.hpp"
#include <glm/fwd.hpp>
#include <memory>
namespace Tower {
enum class TowerType { dart, tack, ice, bomb, spike, glue, boomerang, super };
enum class TowerState { unset, ready, cooldown };
class Range : public Util::GameObject {
private:
  float m_radius;
  std::shared_ptr<Components::CollisionComponent> m_collisionComponent;

public:
  Range(float radius, const Util::PTSDPosition &pos)
      : Util::GameObject(
            std::make_shared<Util::Shape>(Util::ShapeType::Circle,
                                          glm::vec2{radius, radius}),
            5, {0, 0}, false) {
    m_Transform.translation = pos.ToVec2();
  }
  void setPosition(const Util::PTSDPosition &position) {
    m_Transform.translation = position.ToVec2();
    m_collisionComponent->setPosition(position);
  }
  Util::PTSDPosition getPosition() const {
    return Util::PTSDPosition(m_Transform.translation.x,
                              m_Transform.translation.y);
  }
  void setVisible(bool visible) { m_Visible = visible; }
  bool isVisible() const { return m_Visible; }
  void setRadius(float radius) {
    m_radius = radius;
    auto shape = std::dynamic_pointer_cast<Util::Shape>(m_Drawable);
    shape->SetSize({radius, radius});
  }
};
class Body : public Util::GameObject {
private:
  
};
class Tower {
protected:
  TowerType m_type;
  TowerState m_state;
  std::shared_ptr<Util::GameObject> m_body;
  std::shared_ptr<Components::CollisionComponent> m_body_collision;
  std::shared_ptr<Range> m_range;

public:
  virtual void setPosition(const Util::PTSDPosition &position) {
    m_body->m_Transform.translation = position.ToVec2();
    m_body_collision->setPosition(position);
    m_range->setPosition(position);
  }
  virtual Util::PTSDPosition getPosition() const {
    return Util::PTSDPosition(m_body->m_Transform.translation.x,
                              m_body->m_Transform.translation.y);
  }
  virtual void setVisible(bool visible) {
    m_body->SetVisible(visible);
    m_range->setVisible(visible);
  }
};
} // namespace Tower
#endif