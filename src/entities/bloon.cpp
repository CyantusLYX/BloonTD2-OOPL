#include "entities/bloon.hpp"
#include "Util/Image.hpp"
#include "Util/Position.hpp"
#include "Util/Time.hpp"
#include "components/collisionComp.hpp"
#include <memory>

Bloon::Bloon(Bloon::Type type, const Util::PTSDPosition pos)
    : Util::GameObject(nullptr, 10, {0,0}, true),
      Components::CollisionComponent(pos, static_cast<float>(10.0)), // Initialize base class
      m_Type(type), m_State(State::alive) {
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
  m_collisionComponent = std::make_shared<Components::CollisionComponent>(pos, radius);
}

void Bloon::setFrozed(const float froze_time) {
  m_State = State::frozed;
  meltTime = Util::Time::GetElapsedTimeMs() + froze_time;
}

void Bloon::setPosition(const Util::PTSDPosition& position) {
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