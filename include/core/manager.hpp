#ifndef MANAGER_HPP
#define MANAGER_HPP

#include "UI/Flag.hpp"
#include "Core/Drawable.hpp"
#include "UI/SidebarManager.hpp"
#include "UI/buttons/tower_btn.hpp"
#include "UI/buttons/tower_btn_conf.hpp"
#include "UI/buttons/upgrade_btn_conf.hpp"
#include "UI/container/upgrades.hpp"
#include "Util/Color.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Logger.hpp"
#include "Util/Position.hpp"
#include "Util/Renderer.hpp"
#include "Util/SFX.hpp"
#include "Util/Text.hpp"
#include "components/collisionComp.hpp"
#include "components/mortal.hpp"
#include "components/towerType.hpp"
#include "conf.hpp"
#include "core/loader.hpp"
#include "core/shape.hpp"
#include "entities/bloon.hpp"
#include "entities/poppers/glue.hpp"
#include "entities/poppers/popper.hpp"
#include "entities/poppers/spike.hpp"
#include "entities/tower/all_tower.hpp"
#include "entities/tower/tower_config.hpp"
#include "interfaces/clickable.hpp"
#include "interfaces/collision.hpp"
#include "interfaces/draggable.hpp"
#include "interfaces/interfaces.hpp"
#include "interfaces/move.hpp"
#include "map.hpp"
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <glm/fwd.hpp>
#include <magic_enum/magic_enum.hpp>
#include <memory>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

class Manager {
public:
  enum class game_state { playing, gap, menu, over };
  enum class mouse_status { free, drag };
  // game logics
  void updateDraggingObject(const Util::PTSDPosition &cursor_position);
  void processBloonsState();
  void updateAllMovingObjects();
  void handleClickAt(const Util::PTSDPosition &cursor_position);
  void handlePoppers();
  void handleTowers();
  // 生命週期管理
  void register_mortal(std::shared_ptr<Mortal> mortal);
  void cleanup_dead_objects();

  // 氣球控制
  class bloon_holder : public Interface::I_move, public Mortal {
  private:
    std::shared_ptr<Bloon> m_bloon;
    std::vector<std::shared_ptr<Path>> &m_paths; // Reference to manager's paths
    float distance = 0;

  public:
    explicit bloon_holder(std::shared_ptr<Bloon> bloon, float distance,
                          std::vector<std::shared_ptr<Path>> &paths);
    float get_distance();
    Util::PTSDPosition next_position(int frames) override;
    void move() override;
    auto get_bloon() const { return m_bloon; }
    void pre_kill() override { m_bloon->kill(); }
  };

  //  sfx
  class popimg_class : public Mortal, public Util::GameObject {
  public:
    popimg_class()
        : Util::GameObject(
              std::make_shared<Util::Image>(RESOURCE_DIR "/bloons/bpop.png"),
              5) {
      sfx->LoadMedia(RESOURCE_DIR "/sounds/12.mp3");
    };

    std::shared_ptr<Util::GameObject> getobj() {
      return std::make_shared<Util::GameObject>(*this);
    }

    std::shared_ptr<Util::GameObject>
    pop_n_return_img(const Util::PTSDPosition now) {
      sfx->Play();
      this->m_Transform.translation = now.ToVec2();
      return std::make_shared<Util::GameObject>(*this);
    };
    void tick_add() { tick++; }
    int get_tick() { return tick; }
    void voltoggle(bool voltog) {
      if (voltog) {
        sfx->SetVolume(vol);
      } else {
        sfx->SetVolume(0);
      }
    }

  private:
    int vol = 100;
    int tick = 0;
    std::shared_ptr<Util::SFX> sfx =
        std::make_shared<Util::SFX>(RESOURCE_DIR "/sounds");
  };

  class end_spike : public spike {
  public:
    end_spike(const Util::PTSDPosition &pos = {0, 0})
        : spike(pos) {
      setLife(10000000);
    }
  };

  class banners : public Util::GameObject {
  public:
    banners(const std::shared_ptr<Core::Drawable> &drawable, const float zIndex,
            const glm::vec2 &pivot = {0, 0}, const bool visible = true,
            const std::vector<std::shared_ptr<GameObject>> &children =
                std::vector<std::shared_ptr<GameObject>>())
        : Util::GameObject(drawable, zIndex, pivot, visible, children) {}
    bool get_vision() { return m_Visible; }
    void set_ban(bool ban_mode) {
      if (ban_mode == 1)
        m_Drawable =
            std::make_shared<Util::Image>(RESOURCE_DIR "/titles/cl.png");
      else
        m_Drawable =
            std::make_shared<Util::Image>(RESOURCE_DIR "/titles/gg.png");
    }
  };

private:
  std::shared_ptr<Manager::banners> ban;
  int over = -1; //-1:playing 0:gameover 1:win
public:
  void call_ban() {
    if (ban) {
      if (ban->get_vision() == false && m_game_state == game_state::over) {
        ban->set_ban(over == 1 ? 1 : 0);
        ban->SetVisible(true);
      } else if (ban->get_vision() == true &&
                 m_game_state != game_state::over) {
        ban->SetVisible(false);
      }
    }
  }

  // std::shared_ptr<Manager::banners> ban_over =
  // std::make_shared<Manager::banners>(
  //   std::make_shared<Util::Image>(RESOURCE_DIR "/titles/gg.png"), 50.f,
  //   {0, 0}, false, nullptr);

  // 建構函式和解構函式
  explicit Manager(std::shared_ptr<Util::Renderer> &renderer);
  ~Manager() = default;

  // 遊戲物件管理
  void add_bloon(Bloon::Type type, float distance, float z_index = 10);
  void add_bloon(Bloon::Type type, float distance, int path_id,
                 float z_index = 10);
  void add_moving(const std::shared_ptr<Interface::I_move> &moving);
  void add_object(const std::shared_ptr<Util::GameObject> &object);
  void add_popper(const std::shared_ptr<popper> &popper);
  void add_button(const std::shared_ptr<Button> &button);
  void add_updatable(const std::shared_ptr<Interface::IUpdatable> &updatable) {
    updatables.push_back(updatable);
    for (const auto &child : updatable->get_children()) {
      updatables.push_back(child);
    }
  }
  void pop_bloon(std::shared_ptr<bloon_holder> bloon, bool fx = true);
  void set_flag(const std::shared_ptr<UI::Flag> &flag) {
    if (flag != nullptr) {
      m_Renderer->AddChild(flag);
      register_mortal(flag);
    }
    if (current_flag)
      current_flag->kill();
    current_flag = flag;
  }

  void add_tower(const std::shared_ptr<Tower::Tower> &tower);

  void popimg_tick_manager();

  // 遊戲狀態和流程控制
  void next_wave();
  void start_wave();
  void set_map(int diff);
  void set_menu() { m_game_state = game_state::menu; };
  void set_gap() { m_game_state = game_state::gap; };
  void set_playing() { m_game_state = game_state::playing; };
  void wave_check();
  void menu_control(int diff);
  void menu_control(bool visible);
  bool f_wave_end = 0;
  void add_map(const std::shared_ptr<Map> &map);
  void update();
  // 點擊和拖曳相關
  void add_clickable(const std::shared_ptr<Interface::I_clickable> &clickable) {
    clickables.push_back(clickable);
  }
  void set_dragging(const std::shared_ptr<Interface::I_draggable> &draggable);
  void end_dragging(); // ender_dragon()
  bool drag_cd = false;

  // UI 相關方法
  void unselectAll() { unSelectFlag(); };
  void initUI();
  void updateUI();
  void unSelectFlag();

  // 塔建造相關
  void startDraggingTower(Tower::TowerType towerType);
  void placeCurrentTower(const Util::PTSDPosition &position);
  void cancelTowerPlacement();

  // 創建一個塔
  std::shared_ptr<Tower::Tower> createTower(Tower::TowerType type,
                                            const Util::PTSDPosition &position);

  // 在路徑終點創建終極釘子
  void createSpikeAtEnd();

  // Getters 函式
  mouse_status get_mouse_status() const { return m_mouse_status; }
  game_state get_game_state() const { return m_game_state; }
  auto get_movings() { return movings; }
  auto get_clicks() { return clickables; }
  auto get_dragging() { return dragging; }
  auto get_bloons() { return bloons; }
  std::shared_ptr<Map> get_curr_map();
  auto get_current_waves() { return current_waves; }
  int getMoney() const { return money; }
  int getLife() const { return life; }
  int getCurrentWave() const { return current_waves; }

  // some resources
  std::shared_ptr<Util::Text> m_waveText_text = std::make_shared<Util::Text>(
      RESOURCE_DIR "/font/NotoSansTC-ExtraLight.ttf", 32, "0",
      Util::Color(0, 0, 0), false);
  std::shared_ptr<Util::GameObject> m_waveText =
      std::make_shared<Util::GameObject>(m_waveText_text, 5);
  std::shared_ptr<Util::Image> m_gameover_img =
      std::make_shared<Util::Image>(RESOURCE_DIR "/titles/gg.png");
  std::shared_ptr<Util::GameObject> m_gameover =
      std::make_shared<Util::GameObject>(m_gameover_img, 100);
  std::shared_ptr<UI::Flag> current_flag;

private:
  // 渲染和狀態
  std::shared_ptr<Util::Renderer> &m_Renderer;
  mouse_status m_mouse_status = mouse_status::free;
  game_state m_game_state = game_state::menu;
  uintmax_t m_frame_count = 0;

  // 遊戲資源
  std::vector<Bloon::Type> bloons_gen_list;
  int life;
  int money;

  // 地圖和路徑
  std::vector<std::shared_ptr<Map>> maps;
  std::shared_ptr<Map> current_map;
  std::vector<std::shared_ptr<Path>> current_paths;
  std::vector<std::shared_ptr<Path>> all_paths; // Store all available paths

  // 關卡控制
  int current_diff = 0;
  public:
  int get_diff(){
    return current_diff;
  };
  private:
  int bloon_interval = 0;
  int frame_count = 0;
  int current_waves = -1;

  // 所有需要生命週期管理的物件
  std::vector<std::shared_ptr<Mortal>> mortals;

  // 互動物件
  std::vector<std::shared_ptr<Interface::I_move>> movings;
  std::vector<std::shared_ptr<Interface::I_clickable>> clickables;
  std::shared_ptr<Interface::I_draggable> dragging = nullptr;
  std::vector<std::shared_ptr<bloon_holder>> bloons;
  std::vector<std::shared_ptr<popper>> poppers;
  std::vector<std::shared_ptr<Button>> buttons;
  std::vector<std::shared_ptr<Tower::Tower>> towers;
  std::vector<std::shared_ptr<popimg_class>> popimgs;
  std::vector<std::shared_ptr<Interface::IUpdatable>> updatables;

  // UI 相關
  std::shared_ptr<UI::SidebarManager> m_sidebarManager;
  bool m_isTowerDragging = false;
  Tower::TowerType m_dragTowerType;
  int m_dragTowerCost = 0;

public:
  void menu_hover(Util::PTSDPosition now);
  int get_over(){return over;}
  void set_over(int o) { over = o; }

private:
  std::shared_ptr<Button> sound = std::make_shared<Button>(
      "sound", Util::PTSDPosition(-310, 230), glm::vec2(50, 50));
  std::shared_ptr<Button> b_start_round = std::make_shared<Button>(
      "start_round", Util::PTSDPosition(235, -180), glm::vec2(50, 50));
  std::shared_ptr<Button> end_game = std::make_shared<Button>(
      "end_game", Util::PTSDPosition(270, -220), glm::vec2(50, 50));
  std::shared_ptr<Button> infinity = std::make_shared<Button>(
      "infinity", Util::PTSDPosition(30, -220), glm::vec2(50, 50));
  std::shared_ptr<Button> clear = std::make_shared<Button>(
      "clear", Util::PTSDPosition(110, -220), glm::vec2(50, 50));
  std::shared_ptr<Button> skip = std::make_shared<Button>(
      "skip", Util::PTSDPosition(190, -220), glm::vec2(50, 50));
  
public:
  void medal_setter(int diff);
private:
  std::shared_ptr<Util::GameObject> startround_anim;
  // sound->setSize({50, 50});
  // std::shared_ptr<Button> easy_btn = std::make_shared<Button>(
  //   "easy", Util::PTSDPosition(-200, 100), glm::vec2(50, 50));
  std::vector<std::shared_ptr<Button>> emh_menu_buttons;
  std::vector<std::shared_ptr<Util::GameObject>> emh_medals;
  std::shared_ptr<Util::GameObject> menu_bg =
      std::make_shared<Util::GameObject>(std::make_shared<Util::Shape>(Util::ShapeType::Rectangle, glm::vec2(420, 300),
                                    Util::Color(0, 0, 0, 100)), 2.1f);

  std::shared_ptr<popper> createPopper(Tower::TowerType type,
                                       const Util::PTSDPosition &position);
  // 塔工廠函數類型定義
  using TowerFactory =
      std::function<std::shared_ptr<Tower::Tower>(const Util::PTSDPosition &)>;

  // Popper 工廠函數類型定義
  using PopperFactory =
      std::function<std::shared_ptr<popper>(const Util::PTSDPosition &)>;

  // 塔工廠映射表
  std::unordered_map<Tower::TowerType, TowerFactory> m_towerFactories;

  // Popper 工廠映射表
  std::unordered_map<Tower::TowerType, PopperFactory> m_popperFactories;

  // 初始化塔工廠
  void initTowerFactories();
};

#endif // MANAGER_HPP
