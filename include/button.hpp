#ifndef BUTTON_HPP
#define BUTTON_HPP
#include "collapsible.hpp"
#include <variant>
#include <memory>
#include <vector>
//enum class State {non_clickable, clickable, clicked};
class Button final: public Collapsible {
  public:
    Button(const std::string &name, const Util::PTSDPosition pos, std::variant<glm::vec2, int> col_parm, bool can_click);
    ~Button() override = default;
  private:
    //State m_State = State::non_clickable;
    std::string name;
  public:
    //enum class click_state {};
    void when_click_toggle(bool c_toggle);//change image

    //State get_state()const { return this->m_State; }
    //void set_state(State state) { this->m_State = state; }
  };
#endif