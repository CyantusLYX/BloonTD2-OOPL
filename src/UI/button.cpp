#include "UI/button.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Logger.hpp"
#include "Util/Position.hpp"
#include <glm/fwd.hpp>

Button::Button(const std::string &name, const Util::PTSDPosition &pos,
               const std::variant<glm::vec2, float> col_parm = 0.0f,
               bool can_click = true)
    : Components::CollisionComponent(pos, col_parm),
      Util::GameObject(nullptr, 1, {0, 0}, true) {
  this->name = name;
  m_Drawable =
      std::make_shared<Util::Image>(RESOURCE_DIR "/buttons/B" + name + ".png");

      // Set the GameObject transform position - this is the key fix
  m_Transform.translation = pos.ToVec2();
  if (col_parm.index() == 0)
    setColParam(static_cast<float>(m_Drawable->GetSize().x / 2));
  else {
    const glm::vec2 a = {m_Drawable->GetSize().x, m_Drawable->GetSize().y};
    setColParam(a);
    shape.SetSize(a);
  }
}

// void Button::when_click_toggle(const bool c_toggle) {

//   // m_Drawable = std::make_shared<Util::Image>(RESOURCE_DIR "/buttons/b" +
//   name +
//   //                                            (c_toggle ? "c" : "") +
//   ".png");
// } // change image while clicked

void Button::onClick() {
  // 只做簡單的日誌記錄，具體功能由 Manager 處理
  LOG_DEBUG("BUTTON: button " + name + " onClick");
}

void Button::onFocus() { LOG_INFO("BUTTON:" + name + " onFokus"); }

bool Button::isClickable() const { return m_State != State::non_clickable; }

void Button::setClickable(bool clickable) {
  m_State = clickable ? State::clickable : State::non_clickable;  // 修正連字符為下劃線
  LOG_INFO("BUTTON: {} set to {}", name.c_str(),  // 加入 c_str() 確保格式正確
           clickable ? "clickable" : "non-clickable");
}