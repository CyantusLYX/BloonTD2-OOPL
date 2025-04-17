#ifndef TOWER_HPP
#define TOWER_HPP
#include "Core/Drawable.hpp"
#include "Util/GameObject.hpp"
#include "Util/Position.hpp"
#include "components/collisionComp.hpp"
#include "core/path.hpp"
#include "core/shape.hpp"
#include "entities/bloon.hpp"
#include "entities/poppers/popper.hpp"
#include <glm/fwd.hpp>
#include <memory>
#include <vector>
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
                                          glm::vec2{radius*2, radius*2}),
            3, {0, 0}, false) {
    m_Transform.translation = pos.ToVec2();
    m_radius = radius;
    auto shape = std::dynamic_pointer_cast<Util::Shape>(m_Drawable);
    shape->SetColorHSV(0.1f, 0.001f, 0.5f, 0.5f); // 設置顏色和透明度
    m_collisionComponent = std::make_shared<Components::CollisionComponent>(
        pos, radius); // 圓形碰撞組件
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
    m_collisionComponent->setColParam(radius);
  }
  std::shared_ptr<Components::CollisionComponent>
  getCollisionComponent() const {
    return m_collisionComponent;
  }
};
class Body : public Util::GameObject {
private:
  std::shared_ptr<Components::CollisionComponent> m_collisionComponent;

public:
  void setPosition(const Util::PTSDPosition &position) {
    m_Transform.translation = position.ToVec2();
    m_collisionComponent->setPosition(position);
  }
  Util::PTSDPosition getPosition() const {
    return Util::PTSDPosition(m_Transform.translation.x,
                              m_Transform.translation.y);
  }
  Body(const Util::PTSDPosition &pos,
       std::shared_ptr<Core::Drawable> drawable) {
    SetDrawable(drawable);
    auto size_vector = drawable->GetSize();
    m_collisionComponent =
        std::make_shared<Components::CollisionComponent>(pos, size_vector);
    setPosition(pos);
    m_ZIndex = 5;
  };
  void setDrawable(const std::shared_ptr<Core::Drawable> &drawable) {
    m_Drawable = drawable;
  }
};
class Tower {
protected:
  TowerType m_type;
  TowerState m_state;
  std::function<void(std::shared_ptr<popper>)> m_popperCallback;
  std::shared_ptr<Body> m_body;
  std::shared_ptr<Range> m_range;
  std::shared_ptr<Path> m_path;

public:
  virtual void setPosition(const Util::PTSDPosition &position) {
    m_body->setPosition(position);
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

  void setPath(const std::shared_ptr<Path> &path) { m_path = path; }
  virtual std::shared_ptr<Components::CollisionComponent>
  getCollisionComponent() const {
    return m_range->getCollisionComponent();
  }

  void
  setPopperCallback(std::function<void(std::shared_ptr<popper>)> callback) {
    m_popperCallback = callback;
  }

  virtual void
  handleBloonsInRange(const std::vector<std::shared_ptr<Bloon>> &bloons,
                      const std::vector<float> &distances) = 0;

  std::shared_ptr<Path> getPath() const { return m_path; }

  virtual std::shared_ptr<Body> getBody() { return m_body; }

  virtual std::shared_ptr<Range> getRange() { return m_range; }
};
} // namespace Tower
#endif