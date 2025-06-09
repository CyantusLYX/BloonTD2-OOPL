#include "App.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Renderer.hpp"
#include "core/shape.hpp"
#include "entities/bloon.hpp"
#include "entities/poppers/spike.hpp"
#include "test/test.hpp"
#include <Util/Time.hpp>
#include <X11/X.h>
#include <cmath>
#include <glm/fwd.hpp>
#include <imgui.h>
#include <memory>
#include "core/ShapeAnimation.hpp"
bool drag_cd = false;
void App::Start() {
  LOG_TRACE("Start");
  m_CurrentState = State::UPDATE;
  manager->set_map(1);

  manager->add_bloon(Bloon::Type::red, 0);
  manager->set_playing();
  auto first_bloon = manager->get_bloons()[0];
  manager->pop_bloon(first_bloon);
  auto pos_shift = Util::PTSDPosition(0, -5).ToVec2() + manager->get_curr_map()->get_path()->getPositionAtPercentage(1).ToVec2();
  auto spike_at_end = std::make_shared<Manager::end_spike>(Util::PTSDPosition(pos_shift.x, pos_shift.y));
  spike_at_end->setLife(10000000);
  spike_at_end->setCanPopBlack(true);
  spike_at_end->setCanPopFrozen(true);
  spike_at_end->setExplosive(true);
  manager->add_popper(spike_at_end);

  manager->set_menu();
}

void App::Update() {
//   if(manager->get_game_state() == Manager::game_state::over){
//     exit(-1);
//   }
//   else
// {
  manager->cleanup_dead_objects();
  if (manager->get_game_state() == Manager::game_state::menu) {
    auto now = Util::Input::GetCursorPosition();
    if(now.ToVec2() != previous_cursor_pos.ToVec2()){
    previous_cursor_pos = Util::Input::GetCursorPosition();
    manager->menu_hover(now);
  }
  }else if(manager->get_game_state() == Manager::game_state::over){
    
  }
  else if (manager->get_game_state() == Manager::game_state::gap){

  }
  else if (manager->get_game_state() == Manager::game_state::playing) {
    // 更新遊戲邏輯
    //manager->updateDraggingObject(Util::Input::GetCursorPosition());
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
  if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
    m_CurrentState = State::END;
  }
  manager->wave_check();
  manager->update();//}
}

void App::End() { // NOLINT(this method will mutate members in the future)
  LOG_TRACE("End");
}
