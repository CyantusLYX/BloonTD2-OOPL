#include "bloon.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "collapsible.hpp"
#include <memory>
Bloon::Bloon(Bloon::Type type, const glm::vec2 &pos)
   : Collapsible(nullptr, 10, pos, true), m_Type(type) {
//Bloon::Bloon(Bloon::Type type)
//    :  m_Type(type) {
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
  m_Drawable = std::make_shared<Util::Image>(
      RESOURCE_DIR "/bloons/b" + std::to_string(static_cast<int>(m_Type)) +
      ".png");
}
void Bloon::setFrozed(float froze_time) {
  m_State = State::frozed;
  meltTime = Util::Time::GetElapsedTimeMs() + froze_time;
}
bool Bloon::isCollide(const Collapsible &other) const {
  return false;
}
void Bloon::update() {
  if (m_State == State::frozed) {
    if (meltTime < Util::Time::GetElapsedTimeMs()) {
      m_State = State::alive;
    }
  }
  if (m_State == State::alive) {
  }
  if (m_State == State::pop) {
    for (auto &bloon : m_ChildBloons) {
      //std::make_shared<Bloon>(*bloon, m_Pivot);
    }
  }
}