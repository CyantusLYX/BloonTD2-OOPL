#ifndef BLOON_HPP
#define BLOON_HPP
#include "collapsible.hpp"
#include <memory>
#include <vector>
class Bloon final : public Collapsible {
public:
  enum class State { alive, frozed, glued, pop };
  enum class Type { red, blue, green, yellow, black, white, lead, rainbow };

private:
  Bloon::State m_CurrentState = Bloon::State::alive;
  Type m_Type;
  State m_State;
  float baseSpeed = 1.0f; // Base speed of the bloon
  float m_SpeedMult;      // real speed = baseSpeed*m_SpeedMult
  int m_RBE;              // Red Bloon Equivalent
  float meltTime{};
  std::vector<std::shared_ptr<Bloon::Type>> m_ChildBloons;

public:
  Bloon(Type type, Util::PTSDPosition pos);

  void setFrozed(float froze_time);

  [[nodiscard]] State GetCurrentState() const { return m_CurrentState; }

  [[nodiscard]] float GetSpeed() const { return baseSpeed * m_SpeedMult; }

  [[nodiscard]] int GetRBE() const { return m_RBE; }

  [[nodiscard]] Type GetType() const { return m_Type; }

  [[nodiscard]] State GetState() const { return m_State; }
};
#endif // BLOON_HPP
