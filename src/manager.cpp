#include "manager.hpp"
void Manager::set_dragging(std::shared_ptr<Collapsible> dragging) {
  if (m_mouse_status == mouse_status::free) {
    this->dragging = dragging;
    m_mouse_status = mouse_status::drag;
  }
}
void Manager::end_dragging() { //ender_dragon()
  if (m_mouse_status == mouse_status::drag) {
    dragging = nullptr;
    m_mouse_status = mouse_status::free;
  }
}
void Manager::add_map(std::shared_ptr<Map> map) { maps.push_back(map); }