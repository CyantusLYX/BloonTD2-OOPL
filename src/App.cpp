#include "App.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Renderer.hpp"
#include "bloon.hpp"
#include "manager.hpp"
#include <cmath>
#include <glm/fwd.hpp>
#include <memory>

bool drag_cd = false;
void App::Start() {
  LOG_TRACE("Start");
  m_CurrentState = State::UPDATE;

  // auto test = Bloon(Bloon::Type::rainbow);
  auto test = std::make_shared<Bloon>(Bloon::Type::rainbow, glm::vec2{0, 0});
  // auto test = std::make_shared<Collapsible>(nullptr, 10, glm::vec2{0,0},
  // true);
  test->set_can_click(true);
  manager->add_moving(test);
}

void App::Update() {

  if (manager->get_mouse_status() == Manager::mouse_status::drag) {
    manager->get_dragging()->set_position(Util::Input::GetCursorPosition());
  }
  for (auto &move : manager->get_movings()) {
    //move->update();
  }
  if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
    LOG_INFO("Mouse Left Button Pressed");
    drag_cd = false;
    if (manager->get_mouse_status()==Manager::mouse_status::drag && !drag_cd) {
      manager->end_dragging();
      drag_cd = true;
    }
    for (auto &move : manager->get_movings()) { //iterating over all moving
      if (move->get_can_click()&&manager->get_mouse_status()==Manager::mouse_status::free && !drag_cd) {
        if (move->isCollide(Util::Input::GetCursorPosition())) {
          manager->set_dragging(move);
          drag_cd = true;
        }
      }
    }
  }
  if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
    m_CurrentState = State::END;
  }
  m_Renderer->Update();
}

void App::End() { // NOLINT(this method will mutate members in the future)
  LOG_TRACE("End");
}
