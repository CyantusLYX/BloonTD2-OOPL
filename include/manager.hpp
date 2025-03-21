#ifndef MANAGER_HPP
#define MANAGER_HPP
#include "bloon.hpp"
#include "collapsible.hpp"
#include "interfaces.hpp"
#include "map.hpp"
#include <memory>
#include <vector>

class Manager {
public:
  enum class mouse_status { free, drag };

private:
  mouse_status m_mouse_status = mouse_status::free;
  std::vector<std::shared_ptr<Map>> maps;
  std::vector<std::shared_ptr<Collapsible>> movings;
  std::shared_ptr<Collapsible> dragging = nullptr;

public:
  Manager() = default;
  ~Manager() = default;
  void add_map(std::shared_ptr<Map> map);
  void add_bloon(std::shared_ptr<Bloon> bloon);
  void add_moving(std::shared_ptr<Collapsible> moving);
  std::vector<std::shared_ptr<Collapsible>> get_movings() { return movings; };
  mouse_status get_mouse_status() { return m_mouse_status; }
  void set_dragging(std::shared_ptr<Collapsible> dragging);
  std::shared_ptr<Collapsible> get_dragging() { return dragging; }
  void end_dragging();
};
#endif