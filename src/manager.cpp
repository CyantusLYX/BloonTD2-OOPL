#include "manager.hpp"
#include "Util/Image.hpp"
#include "Util/Logger.hpp"
#include "Util/Position.hpp"
#include "Util/Renderer.hpp"
#include "bloon.hpp"
#include "move.hpp"
#include <glm/fwd.hpp>
#include <memory>
glm::vec2 to_pos(glm::vec2 vec) { // from vec2(sdl) to ptsd to vec2, GORGIOUS.
  auto pos = Util::PTSDPosition::FromSDL(vec.x, vec.y);
  return pos.ToVec2();
};
void Manager::set_dragging(const std::shared_ptr<Collapsible> &dragging) {
  if (m_mouse_status == mouse_status::free) {
    LOG_INFO("start dragging");
    this->dragging = dragging;
    m_mouse_status = mouse_status::drag;
  }
}
Manager::Manager(std::shared_ptr<Util::Renderer> &renderer)
    : m_Renderer(renderer) {
  std::vector<std::string> map_paths = {RESOURCE_DIR "/maps/easyFull.png",
                                        RESOURCE_DIR "/maps/medFull.png",
                                        RESOURCE_DIR "/maps/hardFull.png"};
  std::vector<std::vector<glm::vec2>> paths = {
      {to_pos({-19, 236}), to_pos({185, 236}), to_pos({185, 110}),
       to_pos({108, 110}), to_pos({108, 38}), to_pos({412, 38}),
       to_pos({412, 117}), to_pos({290, 117}), to_pos({290, 204}),
       to_pos({420, 204}), to_pos({420, 298}), to_pos({114, 298}),
       to_pos({114, 436}), to_pos({235, 436}), to_pos({235, 346}),
       to_pos({235, 346}), to_pos({348, 346}), to_pos({348, 498})},
      {to_pos({-19, 236}), to_pos({185, 236}), to_pos({185, 110}),
       to_pos({108, 110}), to_pos({108, 38}), to_pos({412, 38}),
       to_pos({412, 117}), to_pos({290, 117}), to_pos({290, 204}),
       to_pos({420, 204}), to_pos({420, 298}), to_pos({114, 298}),
       to_pos({114, 436}), to_pos({235, 436}), to_pos({235, 346}),
       to_pos({235, 346}), to_pos({348, 346}), to_pos({348, 498})},
      {to_pos({-19, 236}), to_pos({185, 236}), to_pos({185, 110}),
       to_pos({108, 110}), to_pos({108, 38}), to_pos({412, 38}),
       to_pos({412, 117}), to_pos({290, 117}), to_pos({290, 204}),
       to_pos({420, 204}), to_pos({420, 298}), to_pos({114, 298}),
       to_pos({114, 436}), to_pos({235, 436}), to_pos({235, 346}),
       to_pos({235, 346}), to_pos({348, 346}), to_pos({348, 498})}};
  for (size_t i = 0; i < map_paths.size(); ++i) {
    auto map =
        std::make_shared<Map>(std::make_shared<Util::Image>(map_paths[i]), 1,
                              std::make_shared<Path>(paths[i], 40), false);
    this->add_map(map);
  }
}
/**
 * @brief ender_dragon(LOL) ;)
 */
void Manager::end_dragging() {
  if (m_mouse_status == mouse_status::drag) {
    LOG_INFO("ender dragoned");
    dragging = nullptr;
    m_mouse_status = mouse_status::free;
  }
}
void Manager::add_map(const std::shared_ptr<Map> &map) {
  maps.push_back(map);
  m_Renderer->AddChild(map);
}

/**
 * @brief Add an object that should automatically move
 *
 * All objects that will automatically move should be added to the manager by
 * this function.
 *
 * @param moving The collapsible object to be added
 */
void Manager::add_moving(const std::shared_ptr<Interface::I_move> &moving) {
  movings.push_back(moving);
}
void Manager::add_object(const std::shared_ptr<Util::GameObject> &object) {
  m_Renderer->AddChild(object);
}
std::shared_ptr<Map> Manager::get_curr_map() { return maps[current_diff]; }
void Manager::set_map(int diff) {
  if (diff != 0 && diff != 1 && diff != 2) {
    LOG_ERROR("Invalid map diff");
    return;
  }
  for (auto &map : maps) {
    map->SetVisible(false);
  }
  maps[diff]->SetVisible(true);
  current_map = maps[diff];
  current_diff = diff;
  current_path = maps[diff]->get_path();
}
void Manager::add_bloon(Bloon::Type type, float distance) {
  auto bloon = std::make_shared<Bloon>(
      type, current_path->getPositionAtDistance(distance));
  auto bloon_holder =
      std::make_shared<Manager::bloon_holder>(bloon, distance, current_path);
  m_Renderer->AddChild(bloon);
  movings.push_back(bloon_holder);
}
Manager::bloon_holder::bloon_holder(std::shared_ptr<Bloon> bloon,
                                    float distance,
                                    const std::shared_ptr<Path> path) {
  m_path = path;
  m_bloon = bloon;
  this->distance = distance;
}
float Manager::bloon_holder::get_distance() { return distance; }
Util::PTSDPosition Manager::bloon_holder::next_position(int frames = 1) {
  return m_path->getPositionAtDistance(m_bloon->GetSpeed() * frames + distance);
}
void Manager::bloon_holder::move() {
  distance += m_bloon->GetSpeed();
  m_bloon->set_position(m_path->getPositionAtDistance(distance));
}