#ifndef BLOON_HPP
#define BLOON_HPP
#include "collapsible.hpp"
#include "mortal.hpp"
#include <memory>
#include <vector>
class Bloon final : public Collapsible,public Mortal {
  // Bloon is a subclass of Collapsible and I_move
  // It represents a moving object in the game, like a bloon in BTD
  // It has its own type, state, speed, and other properties
public:
  enum class State { alive, frozed, glued, pop, died };
  enum class Type { red, blue, green, yellow, black, white, lead, rainbow };

private:
  Bloon::State m_CurrentState = Bloon::State::alive;
  Type m_Type;
  State m_State;
  const float baseSpeed = 0.5f; // Base speed of the bloon
  float m_SpeedMult;      // real speed = baseSpeed*m_SpeedMult
  int m_RBE;              // Red Bloon Equivalent(for life calculation)
  float meltTime{};
  std::vector<std::shared_ptr<Bloon::Type>> m_ChildBloons;

public:
  Bloon(Type type, Util::PTSDPosition pos);

  void setFrozed(float froze_time);
  void set_died() { m_State = State::died; }
  State GetCurrentState() const { return m_CurrentState; }

  float GetSpeed() const { return baseSpeed * m_SpeedMult; }

  int GetRBE() const { return m_RBE; }

  Type GetType() const { return m_Type; }

  State GetState() const { return m_State; }

  std::vector<std::shared_ptr<Bloon::Type>>
  GetChildBloons() const {
    return m_ChildBloons;
  }
  void be_clicked() override {
    if (m_State != State::died) {
      m_State = State::pop;
    }
  }
  State get_state() const { return m_State; }
};
#endif // BLOON_HPP
