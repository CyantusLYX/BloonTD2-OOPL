#include "bloons.hpp"
#include "Util/Time.hpp"
Bloons::Bloons(Type type) : m_Type(type) {
  switch (type) {
  case Type::red:
    m_SpeedMult = 1.0f;
    m_RBE = 1;
    break;
  case Type::blue:
    m_SpeedMult = 1.6f;
    m_RBE = 2;
    m_ChildBloons.push_back(Bloons(Type::red));
    break;
  case Type::green:
    m_SpeedMult = 1.8f;
    m_RBE = 3;
    m_ChildBloons.push_back(Bloons(Type::blue));
    break;
  case Type::yellow:
    m_SpeedMult = 3.2f;
    m_RBE = 4;
    m_ChildBloons.push_back(Bloons(Type::green));
    break;
  case Type::black:
    m_SpeedMult = 1.8f;
    m_RBE = 9;
    for (int _=0; _ < 2; _++)
      m_ChildBloons.push_back(Bloons(Type::yellow));
    break;
  case Type::white:
    m_SpeedMult = 2.0f;
    m_RBE = 9;
    for (int _=0; _ < 2; _++)
      m_ChildBloons.push_back(Bloons(Type::yellow));
    break;
  case Type::lead:
    m_SpeedMult = 1.0f;
    m_RBE = 19;
    for (int _=0; _ < 2; _++)
      m_ChildBloons.push_back(Bloons(Type::yellow));
    break;
  case Type::rainbow:
    m_SpeedMult = 2.2f;
    m_RBE = 37;
    for (int _ = 0; _ < 2; _++) {
      m_ChildBloons.push_back(Bloons(Type::white));
      m_ChildBloons.push_back(Bloons(Type::black));
    }
    break;
  }
}
void Bloons::setFrozed(float time) {
  m_State = State::frozed;
  meltTime = Util::Time::GetElapsedTimeMs() + time;
}
void Bloons::update() {
  if (m_State == State::frozed) {
    if (meltTime < Util::Time::GetElapsedTimeMs()) {
      m_State = State::alive;
    }
  }
  if (m_State == State::alive) {
    
  }
  if (m_State == State::pop) {
    for (auto& bloon : m_ChildBloons) {
      //TODO: generate bloon
      
    }
  }
  
}