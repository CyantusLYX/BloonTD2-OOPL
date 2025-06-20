#include "entities/bloon.hpp"
#include "Util/Image.hpp"
#include "Util/Logger.hpp"
#include "Util/Position.hpp"
#include "Util/SFX.hpp"
#include "Util/Time.hpp"
#include "components/collisionComp.hpp"
#include <memory>

Bloon::Bloon(Bloon::Type type, const Util::PTSDPosition pos, float z_index)
    : Util::GameObject(nullptr, z_index, {0, 0}, true),
      Components::CollisionComponent(
          pos, static_cast<float>(10.0)), // Initialize base class
      m_Type(type),
      m_State(State::alive) {
  m_Transform.translation = pos.ToVec2();
  switch (type) {
  case Type::red:
    m_SpeedMult = 1.0f;
    m_RBE = 1;
    break;
  case Type::blue:
    m_SpeedMult = 1.4f;
    m_RBE = 2;
    m_ChildBloons.push_back(std::make_shared<Bloon::Type>(Type::red));
    break;
  case Type::green:
    m_SpeedMult = 1.8f;
    m_RBE = 3;
    m_ChildBloons.push_back(std::make_shared<Bloon::Type>(Type::blue));
    break;
  case Type::yellow:
    m_SpeedMult = 3.2f;
    m_RBE = 4;
    m_ChildBloons.push_back(std::make_shared<Bloon::Type>(Type::green));
    break;
  case Type::black:
    m_SpeedMult = 1.8f;
    m_RBE = 9;
    for (int _ = 0; _ < 2; _++)
      m_ChildBloons.push_back(std::make_shared<Bloon::Type>(Type::yellow));
    break;
  case Type::white:
    m_SpeedMult = 2.5f;
    m_RBE = 9;
    for (int _ = 0; _ < 2; _++)
      m_ChildBloons.push_back(std::make_shared<Bloon::Type>(Type::yellow));
    break;
  case Type::lead:
    m_SpeedMult = 1.0f;
    m_RBE = 19;
    for (int _ = 0; _ < 2; _++)
      m_ChildBloons.push_back(std::make_shared<Bloon::Type>(Type::yellow));
    break;
  case Type::rainbow:
    m_SpeedMult = 2.2f;
    m_RBE = 37;
    for (int _ = 0; _ < 2; _++) {
      m_ChildBloons.push_back(std::make_shared<Bloon::Type>(Type::white));
      m_ChildBloons.push_back(std::make_shared<Bloon::Type>(Type::black));
    }
    break;
  }

  // 設置可繪製資源
  m_Drawable = std::make_shared<Util::Image>(
      RESOURCE_DIR "/bloons/b" + std::to_string(static_cast<int>(m_Type) + 1) +
      ".png");

  // 建立圓形碰撞組件
  float radius = static_cast<float>(m_Drawable->GetSize().x / 2);
  m_collisionComponent =
      std::make_shared<Components::CollisionComponent>(pos, radius);
}

void Bloon::setFrozed(const int freeze_frames) {
  m_State = State::frozed;
  freeze_counter = freeze_frames;
  // this->m_Drawable=
}

void Bloon::setGlued() {
  // 只有當氣球處於活動狀態時才設為黏滯狀態
  if (m_State == State::alive) {
    m_State = State::glued;
    LOG_TRACE("BLOON : 氣球被黏住了，減速 50%（永久效果）");
    // 可以在此處添加視覺效果，如改變氣球顏色等
  } else if (m_State != State::glued) {
    // 其他狀態不受影響（如凍結）
    LOG_TRACE("BLOON : 氣球當前狀態 {}, 無法應用黏滯效果",
              static_cast<int>(m_State));
  }
  // 如果已經處於黏滯狀態，則不需要做任何事
}

void Bloon::updateFreeze() {
  // 如果處於凍結狀態
  if (m_State == State::frozed) {
    // 減少凍結幀數
    if (freeze_counter > 0) {
      freeze_counter--;
    }

    // 如果計數器為零，解凍氣球
    if (freeze_counter <= 0) {
      // 恢復為正常狀態
      m_State = State::alive;

      // 恢復視覺效果（如果有的話）
    }
  }
}

void Bloon::setPosition(const Util::PTSDPosition &position) {
  m_Transform.translation = position.ToVec2();
  CollisionComponent::setPosition(position);
  if (m_collisionComponent) {
    m_collisionComponent->setPosition(position);
  }
}

Util::PTSDPosition Bloon::getPosition() const {
  return {m_Transform.translation.x, m_Transform.translation.y};
}

void Bloon::onClick() {
  if (m_State != State::died) {
    m_State = State::pop;
  }
}
