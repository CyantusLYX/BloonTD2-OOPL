#include "App.hpp"
#include "Util/GameObject.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Renderer.hpp"
#include "core/ShapeAnimation.hpp"
#include "core/shape.hpp"
#include "entities/bloon.hpp"
#include "entities/poppers/spike.hpp"
#include "test/test.hpp"
#include <Util/Time.hpp>
#include <cmath>
#include <glm/fwd.hpp>
#include <imgui.h>
#include <memory>
bool drag_cd = false;
void App::Start() {
  LOG_TRACE("Start");
  m_CurrentState = State::UPDATE;
  manager->set_map(0);

  manager->add_bloon(Bloon::Type::red, 0);
  manager->set_playing();
  auto first_bloon = manager->get_bloons()[0];
  manager->pop_bloon(first_bloon);

  manager->set_menu();
}

void App::Update() {
  //   if(manager->get_game_state() == Manager::game_state::over){
  //     exit(-1);
  //   }
  //   else
  // {
  //   if(manager->get_game_state() == Manager::game_state::over){
  //     exit(-1);
  //   }
  //   else
  // {
  manager->call_ban();
  manager->cleanup_dead_objects();
  if (manager->get_game_state() == Manager::game_state::menu) {
    auto now = Util::Input::GetCursorPosition();
    if (now.ToVec2() != previous_cursor_pos.ToVec2()) {
      previous_cursor_pos = Util::Input::GetCursorPosition();
      manager->menu_hover(now);
    }
    if(manager->get_over() == 1) {
      manager->medal_setter(manager->get_diff());
    }
    if(manager->get_over() != -1) {
      manager->set_over(-1);
    }

    

  } else if (manager->get_game_state() == Manager::game_state::over) {
  } else if (manager->get_game_state() == Manager::game_state::gap) {
  } else if (manager->get_game_state() == Manager::game_state::playing) {
    if(manager->get_over() != -1) manager->set_over(-1);
    // 更新遊戲邏輯
    // manager->updateDraggingObject(Util::Input::GetCursorPosition());
  }
  manager->processBloonsState();
  manager->handlePoppers();
  manager->handleTowers();
  manager->popimg_tick_manager();
  manager->updateAllMovingObjects();
  manager->updateDraggingObject(Util::Input::GetCursorPosition());

  // 處理輸入
  if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
    LOG_INFO("MOUSE : Left Button Pressed");
    manager->handleClickAt(Util::Input::GetCursorPosition());
  }

  // 檢查遊戲結束條件
  if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
    manager->unselectAll();
  }
  if (Util::Input::IfExit()) {
    m_CurrentState = State::END;
  }
  manager->wave_check();
  manager->update();
}

void App::End() { // NOLINT(this method will mutate members in the future)
  LOG_TRACE("End");
}
