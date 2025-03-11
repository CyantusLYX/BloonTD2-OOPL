#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Renderer.hpp"
#include "bloons.hpp"
#include "map.hpp"
#include <memory>

void App::Start() {
  LOG_TRACE("Start");
  m_CurrentState = State::UPDATE;
  std::shared_ptr<Map> easy_map = std::make_shared<Map>(
      std::make_shared<Util::Image>(RESOURCE_DIR"/maps/easyFull.png"),1);
  std::shared_ptr<Map> med_map = std::make_shared<Map>(
      std::make_shared<Util::Image>(RESOURCE_DIR"/maps/medFull.png"),2,false);
  std::shared_ptr<Map> hard_map = std::make_shared<Map>(
      std::make_shared<Util::Image>(RESOURCE_DIR"/maps/hardFull.png"),3,false);
  m_Renderer.AddChild(easy_map);
  m_Renderer.AddChild(med_map);
  m_Renderer.AddChild(hard_map);
}

void App::Update() {

  // TODO: do your things here and delete this line <3

  /*
   * Do not touch the code below as they serve the purpose for
   * closing the window.
   */
  if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
    m_CurrentState = State::END;
  }
  m_Renderer.Update();
}

void App::End() { // NOLINT(this method will mutate members in the future)
  LOG_TRACE("End");
}
