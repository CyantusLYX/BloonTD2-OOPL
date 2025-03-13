#ifndef BLOONS_HPP
#define BLOONS_HPP
#include "pch.hpp"
#include <memory>
#include <vector>
#include "Util/Time.hpp"
#include "interfaces.hpp"
#include "Util/GameObject.hpp"
class Bloon: public Interface::IUpdatable {
public:
  enum class State { alive, frozed, glued, pop };
  enum class Type { red, blue, green, yellow, black, white, lead, rainbow };
private:
  Bloon::State m_CurrentState = Bloon::State::alive;
  Type m_Type;
  State m_State;
  float baseSpeed=1.0f; //Base speed of the bloon
  float m_SpeedMult; //real speed = baseSpeed*m_SpeedMult
  int m_RBE; //Red Bloon Equivalent
  float meltTime;
  std::vector<std::shared_ptr<Bloon>> m_ChildBloons;
public:
  Bloon(Type type);
  void update() override;

  void setFrozed(float froze_time);

  State GetCurrentState() const { return m_CurrentState; }

  float GetSpeed() const { return baseSpeed*m_SpeedMult; }

  int GetRBE() const { return m_RBE; }

  Type GetType() const { return m_Type; }

  State GetState() const { return m_State; }

  void Destroy(); // NOLINT(readability-convert-member-functions-to-static)
};
#endif // BLOONS_HPP
