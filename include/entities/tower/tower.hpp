#ifndef TOWER_HPP
#define TOWER_HPP
#include "Core/Drawable.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Position.hpp"
#include "components/canBuy.hpp"
#include "components/collisionComp.hpp"
#include "components/mortal.hpp"
#include "components/towerType.hpp"
#include "core/path.hpp"
#include "core/shape.hpp"
#include "entities/bloon.hpp"
#include "entities/poppers/popper.hpp"
#include "interfaces/clickable.hpp"
#include <glm/fwd.hpp>
#include <memory>
#include <string>
#include <vector>

class SpecConfig {
public:
  SpecConfig() = default;
  virtual ~SpecConfig() = default;
};
class BombSpecConfig : public SpecConfig {
public:
  BombSpecConfig(float triggerRadius = 20.0f, float explosionRadius = 60.0f)
      : triggerRadius(triggerRadius),
        explosionRadius(explosionRadius) {};
  float triggerRadius;   // 觸發範圍
  float explosionRadius; // 爆炸範圍
};
namespace Tower {
enum class AtkSpeed { Slow, Normal, Fast, HyperSonic };
struct TowerInfo {
  std::string name;
  AtkSpeed attackSpeed;
  float attackRange;
  bool firstUpgrade;
  bool secondUpgrade;
  int investmentCost;
};

enum class TowerState { unset, ready, cooldown };
class Range : public Util::GameObject, public Mortal {
private:
  float m_radius;
  std::shared_ptr<Components::CollisionComponent> m_collisionComponent;

public:
  void pre_kill() override {
    if (m_collisionComponent) {
      m_collisionComponent = nullptr; // 清除碰撞組件
    }
  }
  Range(float radius, const Util::PTSDPosition &pos)
      : Util::GameObject(
            std::make_shared<Util::Shape>(Util::ShapeType::Circle,
                                          glm::vec2{radius * 2, radius * 2}),
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
    shape->SetColorHSV(0.1f, 0.001f, 0.5f, 0.5f);
    shape->SetSize({radius * 2, radius * 2});
    m_collisionComponent->setColParam(radius);
  }
  std::shared_ptr<Components::CollisionComponent>
  getCollisionComponent() const {
    return m_collisionComponent;
  }
  float getRadius() const { return m_radius; }
};
class Body : public Util::GameObject, public Mortal {
private:
  std::shared_ptr<Components::CollisionComponent> m_collisionComponent;

public:
  void pre_kill() override {
    if (m_collisionComponent) {
      m_collisionComponent = nullptr; // 清除碰撞組件
    }
  }
  auto getCollisionComponent() const { return m_collisionComponent; }
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
class Tower : public CanBuy, public Interface::I_clickable, public Mortal {
protected:
  TowerInfo m_info;
  bool Isupgradable = false;
  bool IsClickable = true;
  TowerType m_type;
  TowerState m_state;
  bool m_previewMode = false; // 添加預覽模式標誌
  std::function<void(std::shared_ptr<popper>)> m_popperCallback;
  std::shared_ptr<Body> m_body;
  std::shared_ptr<Range> m_range;
  std::vector<std::shared_ptr<Path>> m_paths;
  std::shared_ptr<Util::GameObject> renderRoot =
      std::make_shared<Util::GameObject>();

public:
  TowerInfo getInfo() const { return m_info; }
  TowerType getType() const { return m_type; }
  virtual void setFirstUpgrade(int cost) {
    m_info.firstUpgrade = true;
    m_info.investmentCost += cost;
  };
  virtual void setSecondUpgrade(int cost) {
    m_info.secondUpgrade = true;
    m_info.investmentCost += cost;
  };
  virtual bool isUpgradable() const { return Isupgradable; }
  void setClickable(bool clickable) override { IsClickable = clickable; }
  bool isClickable() const override { return IsClickable; }
  void onClick() override {
    if (IsClickable) {
      setPreviewMode(!m_previewMode, false);
    }
  }
  void onFocus() override {}
  bool isClick(const Util::PTSDPosition &pos) override {
    return m_body->getCollisionComponent()->isCollide(pos);
  }
  virtual void setRange(float range) { m_range->setRadius(range); }

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

  void setPath(const std::shared_ptr<Path> &path) {
    m_paths.clear();
    m_paths.push_back(path);
  }

  void setPaths(const std::vector<std::shared_ptr<Path>> &paths) {
    m_paths = paths;
  }
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

  std::shared_ptr<Path> getPath() const {
    return m_paths.empty() ? nullptr : m_paths[0];
  }

  const std::vector<std::shared_ptr<Path>> &getPaths() const { return m_paths; }

  virtual std::shared_ptr<Body> getBody() { return m_body; }

  virtual std::shared_ptr<Range> getRange() { return m_range; }

  // 添加設置狀態的方法
  void setState(TowerState state) { m_state = state; }

  // 獲取當前狀態
  TowerState getState() const { return m_state; }

  // 設置預覽模式
  void setPreviewMode(bool previewMode, bool isUnset = true) {
    m_previewMode = previewMode;

    // 預覽模式下顯示範圍，正常模式下隱藏
    if (m_range) {
      m_range->setVisible(previewMode);
    }
    // 預覽模式下狀態為 unset，否則為 ready
    if (isUnset)
      setState(previewMode ? TowerState::unset : TowerState::ready);
  }

  // 檢查是否為預覽模式
  bool isPreviewMode() const { return m_previewMode; }

  void onDrag(const Util::PTSDPosition &newPosition) override {
    setPosition(newPosition);
  }
  void pre_kill() override {
    if (m_body) {
      m_body->kill();
    }
    if (m_range) {
      m_range->kill();
    }
  }
  void virtual setSpecConfig(std::shared_ptr<SpecConfig> specConfig){};
};
} // namespace Tower
#endif
