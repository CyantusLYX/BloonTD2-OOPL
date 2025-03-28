#include "manager.hpp"
#include "Util/Logger.hpp"
void Manager::set_dragging(std::shared_ptr<Collapsible> dragging) {
  if (m_mouse_status == mouse_status::free) {
    LOG_INFO("stard dragging");
    this->dragging = dragging;
    m_mouse_status = mouse_status::drag;
  }
}
void Manager::end_dragging() { 
  if (m_mouse_status == mouse_status::drag) {
    LOG_INFO("ender dragoned");
    dragging = nullptr;
    m_mouse_status = mouse_status::free;
  }
}
void Manager::add_map(std::shared_ptr<Map> map) { maps.push_back(map); }

void Manager::add_moving(std::shared_ptr<Collapsible> moving) {
  movings.push_back(moving);
}