#include "manager.hpp"
#include "Util/Logger.hpp"
#include "Util/Renderer.hpp"
#include <memory>
#include "Util/Image.hpp"
void Manager::set_dragging(std::shared_ptr<Collapsible> dragging) {
  if (m_mouse_status == mouse_status::free) {
    LOG_INFO("stard dragging");
    this->dragging = dragging;
    m_mouse_status = mouse_status::drag;
  }
}
Manager::Manager(std::shared_ptr<Util::Renderer> &renderer)
    : m_Renderer(renderer) {
  std::vector<std::string> map_paths = {RESOURCE_DIR "/maps/easyFull.png",
                                        RESOURCE_DIR "/maps/medFull.png",
                                        RESOURCE_DIR "/maps/hardFull.png"};
  for (size_t i = 0; i < map_paths.size(); ++i) {
    auto map = std::make_shared<Map>(
        std::make_shared<Util::Image>(map_paths[i]), i + 1, i == 0);
    this->add_map(map);
    m_Renderer->AddChild(map);
  }
}
/**
 * @brief ender_dragon(LOL)
 */
void Manager::end_dragging() {
  if (m_mouse_status == mouse_status::drag) {
    LOG_INFO("ender dragoned");
    dragging = nullptr;
    m_mouse_status = mouse_status::free;
  }
}
void Manager::add_map(std::shared_ptr<Map> map) { maps.push_back(map); }

/**
 * @brief Add an object that should automatically move
 *
 * All objects that will automatically move should be added to the manager by
 * this function.
 *
 * @param moving The collapsible object to be added
 */
void Manager::add_moving(std::shared_ptr<Collapsible> moving) {
  movings.push_back(moving);
  m_Renderer->AddChild(moving);
}