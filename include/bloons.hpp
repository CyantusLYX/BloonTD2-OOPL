#ifndef BLOONS_HPP
#define BLOONS_HPP
#include <vector>
#include "Util/Time.hpp"
#include "interfaces.hpp"
class Bloons: public Interface::IUpdatable {
public:
  enum class State { alive, frozed, glued, pop };
  enum class Type { red, blue, green, yellow, black, white, lead, rainbow };
private:
  Bloons::State m_CurrentState = Bloons::State::alive;
  Type m_Type;
  State m_State;
  float baseSpeed=1.0f; //Base speed of the bloon
  float m_SpeedMult; //real speed = baseSpeed*m_SpeedMult
  int m_RBE; //Red Bloon Equivalent
  float meltTime;
  std::vector<Bloons> m_ChildBloons;
public:
  Bloons(Type type);
  void update() override;

  void setFrozed(float time);

  State GetCurrentState() const { return m_CurrentState; }

  float GetSpeed() const { return baseSpeed*m_SpeedMult; }

  int GetRBE() const { return m_RBE; }

  Type GetType() const { return m_Type; }

  State GetState() const { return m_State; }

  void Destroy(); // NOLINT(readability-convert-member-functions-to-static)
};
#endif // BLOONS_HPP
