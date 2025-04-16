#include "App.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Renderer.hpp"
#include "entities/bloon.hpp"
#include "entities/poppers/spike.hpp"
#include "core/shape.hpp"
#include "test/test.hpp"
#include <Util/Time.hpp>
#include <cmath>
#include <glm/fwd.hpp>
#include <memory>
bool drag_cd = false;
void App::Start() {
  LOG_TRACE("Start");
  m_CurrentState = State::UPDATE;
  manager->set_map(0);
  auto test_drawable = std::make_shared<Util::Shape>(Util::ShapeType::Circle,
                                                     glm::vec2(100.0f, 100.0f));
  test_drawable->SetColorHSV(0.0f, 1.0f, 1.0f, 0.3f);
  auto test = std::make_shared<coshader>(test_drawable, 5);
  manager->add_object(test);
  manager->add_bloon(Bloon::Type::red, 0);
  manager->set_playing();
  auto first_bloon = manager->get_bloons()[0];
  manager->pop_bloon(first_bloon);
  auto first_spike = std::make_shared<spike>(Util::PTSDPosition(-135,4));
  //auto first_spike = std::make_shared<spike>(Util::PTSDPosition(0,0));
  manager->add_popper(first_spike);
}

void App::Update() {
  manager->cleanup_dead_objects();
  if (manager->get_game_state() != Manager::game_state::menu) {
    // 更新遊戲邏輯
    manager->updateDraggingObject(Util::Input::GetCursorPosition());
    manager->processBloonsState();
    manager->updateAllMovingObjects();
    manager->handlePoppers();
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
  manager->wave_check();
  manager->update();
}

void App::End() { // NOLINT(this method will mutate members in the future)
  LOG_TRACE("End");
}
