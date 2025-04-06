#include "button.hpp"
#include "Util/Image.hpp"
#include "Util/Logger.hpp"
#include "Util/Position.hpp"
#include <glm/fwd.hpp>

Button::Button(const std::string &name, const Util::PTSDPosition &pos, const std::variant<glm::vec2, int> col_parm = 0, bool can_click = true)
: Collapsible(nullptr, 10, pos, col_parm){
  this->name = name;
  m_Drawable = std::make_shared<Util::Image>(
    RESOURCE_DIR "/buttons/b" + name +
    ".png");
  if (col_parm.index() == 0) set_col_parm(static_cast<int>(m_Drawable->GetSize().x / 2));
  else {
    const glm::vec2 a = {m_Drawable->GetSize().x, m_Drawable->GetSize().y};
    set_col_parm(a);
  }
}

void Button::when_click_toggle(const bool c_toggle){
  m_Drawable = std::make_shared<Util::Image>(
    RESOURCE_DIR "/buttons/b" + name + (c_toggle ? "c" : "") +
    ".png");
}//change image while clicked