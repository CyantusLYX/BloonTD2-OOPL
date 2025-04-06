#ifndef MANAGER_HPP
#define MANAGER_HPP
#include "Util/Renderer.hpp"
#include "bloon.hpp"
#include "collapsible.hpp"
#include "interfaces.hpp"
#include "map.hpp"
#include "move.hpp"
#include <memory>
#include <vector>
class Manager {
public:
  enum class mouse_status { free, drag };

private:
  std::shared_ptr<Util::Renderer> &m_Renderer;
  mouse_status m_mouse_status = mouse_status::free;
  std::vector<std::shared_ptr<Map>> maps;
  std::vector<std::shared_ptr<Interface::I_move>> movings;
  std::shared_ptr<Collapsible> dragging = nullptr;
  void add_map(const std::shared_ptr<Map> &map);
  int life;
  int money;
  int current_diff = 0;
  int bloon_interval = 0;
  int frame_count = 0;
  int current_waves = -1;
  std::shared_ptr<Map> current_map;
  std::shared_ptr<Path> current_path;
  class bloon_holder : public Interface::I_move {
  private:
    std::shared_ptr<Bloon> m_bloon;
    std::shared_ptr<Path> m_path;
    float distance = 0;

  public:
    explicit bloon_holder(std::shared_ptr<Bloon> bloon, float distance,
                          const std::shared_ptr<Path> path);
    float get_distance();
    Util::PTSDPosition next_position(int frames) override;
    void move() override;
  };

protected:
public:
  explicit Manager(std::shared_ptr<Util::Renderer> &renderer);
  ~Manager() = default;
  void add_bloon(Bloon::Type type, float distance);
  void add_moving(const std::shared_ptr<Interface::I_move> &moving);
  void add_object(const std::shared_ptr<Util::GameObject> &object);
  auto get_movings() { return movings; };
  [[nodiscard]] mouse_status get_mouse_status() const { return m_mouse_status; }
  void set_dragging(const std::shared_ptr<Collapsible> &dragging);
  auto get_dragging() { return dragging; }
  void end_dragging(); // ender_dragon()
  void next_level();
  void set_map(int diff);
  std::shared_ptr<Map> get_curr_map();
};
#endif