#include "App.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Renderer.hpp"
#include "Util/Time.hpp"
#include "bloon.hpp"
#include <cmath>
#include <glm/fwd.hpp>
#include <Util/Time.hpp>
bool drag_cd = false;
void App::Start() {
  LOG_TRACE("Start");
  m_CurrentState = State::UPDATE;
  manager->set_map(0);
  manager->add_bloon(Bloon::Type::red, 10);
  manager->add_bloon(Bloon::Type::green, 10);
  manager->add_bloon(Bloon::Type::rainbow, 10);
  manager->next_wave();
}

void App::Update() {
  if (manager->get_game_state() != Manager::game_state::menu) {
    // 更新遊戲邏輯
    manager->updateDraggingObject(Util::Input::GetCursorPosition());
    manager->processBloonsState();
    manager->updateAllMovingObjects();
    manager->cleanup_dead_objects();
  }

  // 處理輸入
  if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
    LOG_INFO("Mouse Left Button Pressed");
    manager->handleClickAt(Util::Input::GetCursorPosition());
  }

  // 檢查遊戲結束條件
  if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
    m_CurrentState = State::END;
  }
  // 更新渲染
  m_Renderer->Update();
}

void App::End() { // NOLINT(this method will mutate members in the future)
  LOG_TRACE("End");
}
