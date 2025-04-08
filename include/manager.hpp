#ifndef MANAGER_HPP
#define MANAGER_HPP

#include "Util/Renderer.hpp"
#include "bloon.hpp"
#include "collapsible.hpp"
#include "map.hpp"
#include "mortal.hpp"
#include "move.hpp"
#include <memory>
#include <vector>

class Manager {
public:
  enum class mouse_status { free, drag };
  // game logics
  void updateDraggingObject(const Util::PTSDPosition &cursor_position);
  void processBloonsState();
  void updateAllMovingObjects();
  void handleClickAt(const Util::PTSDPosition &cursor_position);

  // 生命週期管理
  void register_mortal(std::shared_ptr<Mortal> mortal);
  void cleanup_dead_objects();

  // 氣球控制
  class bloon_holder : public Interface::I_move {
  public:
    explicit bloon_holder(std::shared_ptr<Bloon> bloon, float distance,
                          const std::shared_ptr<Path> path);
    float get_distance();
    Util::PTSDPosition next_position(int frames) override;
    void move() override;
    auto get_bloon() const { return m_bloon; }

  private:
    std::shared_ptr<Bloon> m_bloon;
    std::shared_ptr<Path> m_path;
    float distance = 0;
  };

  // 建構函式和解構函式
  explicit Manager(std::shared_ptr<Util::Renderer> &renderer);
  ~Manager() = default;

  // 遊戲物件管理
  void add_bloon(Bloon::Type type, float distance);
  void add_moving(const std::shared_ptr<Interface::I_move> &moving);
  void add_object(const std::shared_ptr<Util::GameObject> &object);
  void pop_bloon(std::shared_ptr<bloon_holder> bloon);

  // 遊戲狀態和流程控制
  void next_level();
  void set_map(int diff);

  // 點擊和拖曳相關
  void add_click(const std::shared_ptr<Collapsible> &click) {
    clicks.push_back(click);
  }
  void set_dragging(const std::shared_ptr<Collapsible> &dragging);
  void end_dragging(); // ender_dragon()

  // Getter 函式
  mouse_status get_mouse_status() const { return m_mouse_status; }
  auto get_movings() { return movings; }
  auto get_clicks() { return clicks; }
  auto get_dragging() { return dragging; }
  auto get_bloons() { return bloons; }
  std::shared_ptr<Map> get_curr_map();

private:
  // 渲染和狀態
  std::shared_ptr<Util::Renderer> &m_Renderer;
  mouse_status m_mouse_status = mouse_status::free;

  // 遊戲資源
  int life;
  int money;

  // 地圖和路徑
  std::vector<std::shared_ptr<Map>> maps;
  std::shared_ptr<Map> current_map;
  std::shared_ptr<Path> current_path;
  void add_map(const std::shared_ptr<Map> &map);

  // 關卡控制
  int current_diff = 0;
  int bloon_interval = 0;
  int frame_count = 0;
  int current_waves = -1;

  // 所有需要生命週期管理的物件
  std::vector<std::shared_ptr<Mortal>> mortals;

  // 互動物件
  std::vector<std::shared_ptr<Interface::I_move>> movings;
  std::vector<std::shared_ptr<Collapsible>> clicks;
  std::shared_ptr<Collapsible> dragging = nullptr;
  std::vector<std::shared_ptr<bloon_holder>> bloons;
};

#endif // MANAGER_HPP