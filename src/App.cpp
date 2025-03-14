#include "App.hpp"

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Renderer.hpp"
#include "bloon.hpp"
#include "collapsible.hpp"
#include "map.hpp"
#include <glm/fwd.hpp>
#include <memory>

void App::Start() {
  LOG_TRACE("Start");
  m_CurrentState = State::UPDATE;
  auto easy_map = std::make_shared<Map>(
      std::make_shared<Util::Image>(RESOURCE_DIR"/maps/easyFull.png"),1);
      auto med_map = std::make_shared<Map>(
      std::make_shared<Util::Image>(RESOURCE_DIR"/maps/medFull.png"),2,false);
      auto hard_map = std::make_shared<Map>(
      std::make_shared<Util::Image>(RESOURCE_DIR"/maps/hardFull.png"),3,false);
  m_Renderer.AddChild(easy_map);
  m_Renderer.AddChild(med_map);
  m_Renderer.AddChild(hard_map);
  auto test = Bloon(Bloon::Type::rainbow);
  //auto test = Bloon(Bloon::Type::rainbow, glm::vec2{0,0});
  //auto test = std::make_shared<Collapsible>(nullptr, 10, glm::vec2{0,0}, true);
  //m_Renderer.AddChild(test);
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
