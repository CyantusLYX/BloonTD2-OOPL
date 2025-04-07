#include "App.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Renderer.hpp"
#include "bloon.hpp"
#include "manager.hpp"
#include <cmath>
#include <glm/fwd.hpp>

bool drag_cd = false;
void App::Start() {
  LOG_TRACE("Start");
  m_CurrentState = State::UPDATE;

  // auto test = Bloon(Bloon::Type::rainbow);
  // auto test = std::make_shared<Collapsible>(nullptr, 10, glm::vec2{0,0},
  // true);
  manager->set_map(0);
  manager->add_bloon(Bloon::Type::red, 10);
  manager->add_bloon(Bloon::Type::green, 10);
  
}

void App::Update() {

  if (manager->get_mouse_status() == Manager::mouse_status::drag) {
    manager->get_dragging()->set_position(Util::Input::GetCursorPosition());
  }
  for (auto &bloon : manager->get_bloons()){
    if (bloon->get_bloon()->get_state() == Bloon::State::pop) {
      manager->pop_bloon(bloon.get());
    }
  }
  for (auto &move : manager->get_movings()) {
    move->move();
  }
  if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
    LOG_INFO("Mouse Left Button Pressed");
    drag_cd = false;
    if (manager->get_mouse_status() == Manager::mouse_status::drag &&
        !drag_cd) {
      manager->end_dragging();
      drag_cd = true;
    }
    for (auto &click : manager->get_clicks()) { // iterating over all moving
      if (!(click->get_can_click()&&!drag_cd)) {
        continue;
      }
      if (click->isCollide(Util::Input::GetCursorPosition())) {
        if (click->get_can_drag()) {
          if (manager->get_mouse_status() == Manager::mouse_status::free &&
              !drag_cd) {
            manager->set_dragging(click);
            drag_cd = true;
          }
        }
      if(click->get_can_click()) {
          click->be_clicked();
          LOG_INFO("Clicked");
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
