#ifndef BUTTON_HPP
#define BUTTON_HPP
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "components/collisionComp.hpp"
#include "core/shape.hpp"
#include "interfaces/clickable.hpp"
#include <memory>
#include <variant>
#include <vector>

enum class State { non_clickable, clickable, clicked };
class Button final : public Components::CollisionComponent,
                     public Util::GameObject,
                     public Interface::I_clickable {
public:
  Button(const std::string &name, const Util::PTSDPosition &pos,
         std::variant<glm::vec2, float> col_parm, bool can_click);
  ~Button() override = default;

private:
  Util::Shape shape = Util::Shape(std::get_if<float>(&m_colParam) == nullptr
                                      ? Util::ShapeType::Rectangle
                                      : Util::ShapeType::Circle,
                                  {10.0, 10.0});
  State m_State = State::clickable;
  std::string name;

public:
  // enum class click_state {};
  //void when_click_toggle(bool c_toggle); // change image

  virtual void onClick() override;
  virtual void onFocus() override;

  virtual bool isClickable() const override;
  virtual void setClickable(bool clickable) override;

  // State get_state()const { return this->m_State; }
  // void set_state(State state) { this->m_State = state; }

  const std::string &getName() const { return name; }
};
#endif