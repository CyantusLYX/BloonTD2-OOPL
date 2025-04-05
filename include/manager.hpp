#ifndef MANAGER_HPP
#define MANAGER_HPP
#include "Util/Renderer.hpp"
#include "bloon.hpp"
#include "collapsible.hpp"
#include "map.hpp"
#include <memory>
#include <vector>

class Manager {
public:
  enum class mouse_status { free, drag };
private:
  std::shared_ptr<Util::Renderer> &m_Renderer;
  mouse_status m_mouse_status = mouse_status::free;
  std::vector<std::shared_ptr<Map>> maps;
  std::vector<std::shared_ptr<Collapsible>> movings;
  std::shared_ptr<Collapsible> dragging = nullptr;
  void add_map(const std::shared_ptr<Map>& map);
  int life{};
  int money{};
  int current_level = 0;
  int bloon_interval = 0;
  int frame_count = 0;

public:
  explicit Manager(std::shared_ptr<Util::Renderer> &renderer);
  ~Manager() = default;
  void add_bloon(std::shared_ptr<Bloon> bloon);
  void add_moving(const std::shared_ptr<Collapsible>& moving);
  auto get_movings() { return movings; };
  [[nodiscard]] mouse_status get_mouse_status() const { return m_mouse_status; }
  void set_dragging(const std::shared_ptr<Collapsible> &dragging);
  auto get_dragging() { return dragging; }
  void end_dragging(); // ender_dragon()
  void next_level();
};
#endif