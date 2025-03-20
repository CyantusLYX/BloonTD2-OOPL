#ifndef MANAGER_HPP
#define MANAGER_HPP
#include "bloon.hpp"
#include "collapsible.hpp"
#include "interfaces.hpp"
#include "map.hpp"
#include <memory>
#include <vector>

class Manager : public Interface::IUpdatable {
private:
  enum class mouse_status {free, drag};
  mouse_status m_mouse_status = mouse_status::free;
  std::vector<std::shared_ptr<Map>> maps;
  std::vector<std::shared_ptr<Collapsible>> movings;
  std::shared_ptr<Collapsible> dragging = nullptr;
public:
  Manager();
  ~Manager();
  void add_map(std::shared_ptr<Map> map);
  void add_bloon(std::shared_ptr<Bloon> bloon);
  void update() override;
  mouse_status get_mouse_status() { return m_mouse_status; }
  void set_dragging(std::shared_ptr<Collapsible> dragging);
  void end_dragging();
};
#endif