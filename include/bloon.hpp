#ifndef BLOON_HPP
#define BLOON_HPP
#include "collapsible.hpp"
#include <memory>
#include <vector>
class Bloon final : public Collapsible{
  // Bloon is a subclass of Collapsible and I_move
  // It represents a moving object in the game, like a bloon in BTD
  // It has its own type, state, speed, and other properties
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

  [[nodiscard]] std::vector<std::shared_ptr<Bloon::Type>> GetChildBloons() const {
    return m_ChildBloons;
  }
};
#endif // BLOON_HPP
