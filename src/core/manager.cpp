#include "core/manager.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Logger.hpp"
#include "Util/Position.hpp"
#include "Util/Renderer.hpp"
#include "Util/Text.hpp"
#include "components/collisionComp.hpp"
#include "core/loader.hpp"
#include "entities/bloon.hpp"
#include "interfaces/clickable.hpp"
#include "interfaces/collision.hpp"
#include "interfaces/draggable.hpp"
#include "interfaces/move.hpp"
#include <algorithm>
#include <chrono>
#include <glm/fwd.hpp>
#include <memory>
#include <random>
#include <vector>

#include <magic_enum/magic_enum.hpp>

bool toggle_show_collision_at = 1;
bool toggle_show_bloons = 1;

// 座標轉換輔助函數
glm::vec2 to_pos(glm::vec2 vec) { // from vec2(sdl) to ptsd to vec2
  auto pos = Util::PTSDPosition::FromSDL(vec.x, vec.y);
  return pos.ToVec2();
}

// Manager 建構函數
Manager::Manager(std::shared_ptr<Util::Renderer> &renderer)
    : m_Renderer(renderer) {
  // 初始化地圖路徑
  std::vector<std::string> map_paths = {RESOURCE_DIR "/maps/easyFull.png",
                                        RESOURCE_DIR "/maps/medFull.png",
                                        RESOURCE_DIR "/maps/hardFull.png"};

  // 定義路徑點
  std::vector<std::vector<glm::vec2>> paths = {
      {to_pos({-19, 236}), to_pos({185, 236}), to_pos({185, 110}),
       to_pos({108, 110}), to_pos({108, 38}), to_pos({412, 38}),
       to_pos({412, 117}), to_pos({290, 117}), to_pos({290, 204}),
       to_pos({420, 204}), to_pos({420, 298}), to_pos({114, 298}),
       to_pos({114, 436}), to_pos({235, 436}), to_pos({235, 346}),
       to_pos({235, 346}), to_pos({348, 346}), to_pos({348, 498})},
      // 中等難度地圖路徑 (目前與簡單難度相同)
      {to_pos({-19, 236}), to_pos({185, 236}), to_pos({185, 110}),
       to_pos({108, 110}), to_pos({108, 38}), to_pos({412, 38}),
       to_pos({412, 117}), to_pos({290, 117}), to_pos({290, 204}),
       to_pos({420, 204}), to_pos({420, 298}), to_pos({114, 298}),
       to_pos({114, 436}), to_pos({235, 436}), to_pos({235, 346}),
       to_pos({235, 346}), to_pos({348, 346}), to_pos({348, 498})},
      // 困難地圖路徑 (目前與簡單難度相同)
      {to_pos({-19, 236}), to_pos({185, 236}), to_pos({185, 110}),
       to_pos({108, 110}), to_pos({108, 38}), to_pos({412, 38}),
       to_pos({412, 117}), to_pos({290, 117}), to_pos({290, 204}),
       to_pos({420, 204}), to_pos({420, 298}), to_pos({114, 298}),
       to_pos({114, 436}), to_pos({235, 436}), to_pos({235, 346}),
       to_pos({235, 346}), to_pos({348, 346}), to_pos({348, 498})}};

  // 創建並添加地圖
  for (size_t i = 0; i < map_paths.size(); ++i) {
    auto map =
        std::make_shared<Map>(std::make_shared<Util::Image>(map_paths[i]), 1,
                              std::make_shared<Path>(paths[i], 40), false);
    this->add_map(map);
  }

  // Add spike button to the top-right corner with adjusted position
  auto spike_button =
      std::make_shared<Button>("spike", Util::PTSDPosition(280, 200), 50.0f,
                               true); // Adjusted position to be visible
  LOG_INFO("MNGR  : Adding spike button at visible top-right corner");
  this->add_button(spike_button);
  this->add_clickable(spike_button);

  //m_waveText->SetDrawable(nullptr);//std::make_shared<Util::Text>(
  //    Util::Text("/usr/share/fonts/gsfonts/C059-Roman.otf", 20, "0", Util::Color(255, 255, 255), false)));
  //m_Renderer->AddChild(m_waveText);
}

// 地圖管理相關函數
void Manager::add_map(const std::shared_ptr<Map> &map) {
  maps.push_back(map);
  m_Renderer->AddChild(map);
}

void Manager::set_map(int diff) {
  if (diff != 0 && diff != 1 && diff != 2) {
    LOG_ERROR("MNGR  : Invalid map diff");
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

std::shared_ptr<Map> Manager::get_curr_map() { return maps[current_diff]; }

// 物件管理相關函數
void Manager::add_object(const std::shared_ptr<Util::GameObject> &object) {
  m_Renderer->AddChild(object);
}

/**
 * @brief 添加自動移動的物件
 * 所有應該自動移動的物件都應使用此函數添加到管理器中
 * @param moving 要添加的可移動物件
 */
void Manager::add_moving(const std::shared_ptr<Interface::I_move> &moving) {
  movings.push_back(moving);
}

// 氣球管理函數
void Manager::add_bloon(Bloon::Type type, float distance,float z_index) {
  auto bloon = std::make_shared<Bloon>(
      type, current_path->getPositionAtDistance(distance),z_index);
  bloon->setClickable(true);  // 更改為使用新介面方法
  this->add_clickable(bloon); // 使用新的方法

  register_mortal(bloon);

  auto bloon_holder =
      std::make_shared<Manager::bloon_holder>(bloon, distance, current_path);

  m_Renderer->AddChild(bloon);
  movings.push_back(bloon_holder);
  bloons.push_back(bloon_holder);
}

void Manager::add_button(const std::shared_ptr<Button> &button) {
  buttons.push_back(button);

  m_Renderer->AddChild(button); // 將按鈕加入渲染器
}

void Manager::pop_bloon(std::shared_ptr<bloon_holder> bloon) {
  bloon->get_bloon()->SetVisible(false);
  if (toggle_show_bloons)
    LOG_INFO("MNGR  : Pop bloon {}",
             std::string(magic_enum::enum_name(bloon->get_bloon()->GetType())));

  // 產生一個不重複的 1~4 順序
  std::vector<int> values = {1, 2, 3, 4};
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle(values.begin(), values.end(), std::default_random_engine(seed));

  // 處理子氣球
  auto sub_bloons = bloon->get_bloon()->GetChildBloons();
  for (size_t i = 0; i < sub_bloons.size() && i < values.size(); ++i) {
    float distance = bloon->get_distance() - values[i] * 5;
    if (toggle_show_bloons)
      LOG_INFO(
          "MNGR  : Gen bloon {} at distance {}",
          std::string(magic_enum::enum_name(bloon->get_bloon()->GetType())),
          distance);
    this->add_bloon(*sub_bloons[i], distance,11+frame_count/10.0f);
  }

  bloon->kill();
}

void Manager::add_popper(const std::shared_ptr<popper> &popper) {
  if (popper->get_object())
    m_Renderer->AddChild(popper->get_object());
  poppers.push_back(popper);
  register_mortal(popper);

  // 若 popper 實現了 I_clickable 介面，則加入可點擊物件列表
  auto clickable_popper =
      std::dynamic_pointer_cast<Interface::I_clickable>(popper);
  if (clickable_popper) {
    this->add_clickable(clickable_popper);
  }
}

// 新增: 創建可拖曳的釘子
void Manager::createDraggableSpike(const Util::PTSDPosition &position) {
  // 創建一個釘子
  auto new_spike = std::make_shared<spike>(position);
  new_spike->setDraggable(true); // 設置為可拖曳

  // 加入管理器
  add_popper(new_spike);

  // 設置為當前拖曳物件
  set_dragging(std::static_pointer_cast<Interface::I_draggable>(new_spike));

  LOG_DEBUG("MNGR  : 創建一個可拖曳的釘子在位置 ({}, {})", position.x,
            position.y);
}

// 修改按鈕點擊處理
// wtbi
void Manager::handleButtonClicks(const Util::PTSDPosition &cursor_position) {
  for (auto &button : buttons) {
    if (button->isClickable() && button->isCollide(cursor_position)) {
      button->onClick();

      // 根據按鈕名稱執行特定操作
      if (button->getName() == "spike") {
        createDraggableSpike(cursor_position);
      }

      LOG_INFO("MNGR  : 按鈕 {} 被點擊了", button->getName().c_str());
      break;
    }
  }
}

// 修改點擊處理，確保第二次點擊時固定位置
void Manager::handleClickAt(const Util::PTSDPosition &cursor_position) {
  static bool drag_cooldown = false;

  // 重置冷卻
  drag_cooldown = false;

  // 如果正在拖曳，則結束拖曳 (第二次點擊時)
  if (m_mouse_status == mouse_status::drag) {
    if (dragging) {
      // 更新最後位置為點擊位置
      dragging->onDrag(cursor_position);
    }
    end_dragging();
    return;
  }

  // 檢查是否點擊了可互動物件
  for (auto &clickable : clickables) {
    // 跳過不可點擊或已在冷卻的物件
    if (!(clickable->isClickable() && !drag_cooldown)) {
      continue;
    }
    if (toggle_show_collision_at)
      LOG_DEBUG("MNGR  : clickable");
    // 獲取具有碰撞功能的 GameObject
    /* auto gameObject = std::dynamic_pointer_cast<Util::GameObject>(clickable);
    if (!gameObject)
      continue; */

    // 檢查物件是否能夠進行碰撞檢測
    bool isCollided = false;

    // 透過介面來檢查碰撞，而不是強制轉換為特定類型
    auto collidable =
        std::dynamic_pointer_cast<Interface::I_collider>(clickable);
    if (collidable) {
      isCollided = collidable->isCollide(cursor_position);
    }

    if (isCollided) {
      // 處理可拖曳物件
      LOG_DEBUG("MNGR  : clicked");
      auto draggable =
          std::dynamic_pointer_cast<Interface::I_draggable>(clickable);
      if (draggable && draggable->isDraggable() &&
          m_mouse_status == mouse_status::free && !drag_cooldown) {
        set_dragging(draggable);
        drag_cooldown = true;
      }

      // 處理點擊事件
      if (clickable->isClickable()) {
        clickable->onClick();
        LOG_DEBUG("MNGR  : Clicked");
      }
      break;
    }
  }
}

// 處理 popper 物件
void Manager::handlePoppers() {
  for (auto &popper : poppers) {
    // 檢查 popper 是否存活且在路徑上
    if (popper->is_alive() && current_path->isOnPath(popper->get_position())) {
      // 收集與 popper 碰撞的氣球
      std::vector<std::shared_ptr<Bloon>> collided_bloons;
      std::vector<std::shared_ptr<bloon_holder>> collided_holders;

      // 檢查所有氣球是否與 popper 碰撞
      for (auto &holder : bloons) {
        auto bloon = holder->get_bloon();
        if (toggle_show_collision_at)
          LOG_DEBUG(
              "MNGR  : Checking collision with bloon at ({},{}) , ({},{})",
              bloon->getPosition().x, bloon->getPosition().y,
              popper->getPosition().x, popper->getPosition().y);
        // 使用氣球的碰撞檢測與 popper 的位置進行碰撞檢測
        if (bloon->isCollide(popper->get_position())) {
          collided_bloons.push_back(bloon);
          collided_holders.push_back(holder);
        }
      }

      // 如果有碰撞的氣球，調用 hit() 函數
      if (!collided_bloons.empty()) {
        auto hit_results = popper->hit(collided_bloons);

        // 處理被擊中的氣球
        for (size_t i = 0; i < hit_results.size(); ++i) {
          if (hit_results[i]) {
            // 使用 pop_bloon 來處理被擊中的氣球，而不是僅設置狀態
            pop_bloon(collided_holders[i]);
          }
        }
      }
      if (popper->is_dead()) {
        // popper->get_object()->SetVisible(false);
        m_Renderer->RemoveChild(popper->get_object());
      }
    }
  }
}

void Manager::register_mortal(std::shared_ptr<Mortal> mortal) {
  mortals.push_back(mortal);
}

void Manager::cleanup_dead_objects() {
  // 首先找出所有已死亡的物件的 UUID
  std::vector<std::string> dead_uuids;
  for (const auto &mortal : mortals) {
    if (mortal->is_dead()) {
      dead_uuids.push_back(mortal->get_uuid());
    }
  }

  // 從 mortals 容器中移除死亡物件
  mortals.erase(
      std::remove_if(mortals.begin(), mortals.end(),
                     [](const auto &mortal) { return mortal->is_dead(); }),
      mortals.end());

  // 從其他容器中移除死亡物件，使用 UUID 匹配

  auto cleanup_container = [&dead_uuids](auto &container, auto extractor) {
    container.erase(std::remove_if(container.begin(), container.end(),
                                   [&dead_uuids, &extractor](const auto &item) {
                                     auto mortal = extractor(item);
                                     if (mortal) {
                                       return std::find(dead_uuids.begin(),
                                                        dead_uuids.end(),
                                                        mortal->get_uuid()) !=
                                              dead_uuids.end();
                                     }
                                     return false;
                                   }),
                    container.end());
  };
  // 使用通用函數清理各個容器
  cleanup_container(bloons, [](const auto &bloon) {
    return std::dynamic_pointer_cast<Mortal>(bloon->get_bloon());
  });

  cleanup_container(movings, [](const auto &moving) {
    return std::dynamic_pointer_cast<Mortal>(moving);
  });

  cleanup_container(clickables, [](const auto &clickable) {
    return std::dynamic_pointer_cast<Mortal>(clickable);
  });
}

void Manager::add_tower(const std::shared_ptr<Tower::Tower> &tower) {
  // 設置回調函數，讓塔可以創建飛鏢
  tower->setPopperCallback([this](std::shared_ptr<popper> p) {
    this->add_popper(p);
    auto move_popper = std::dynamic_pointer_cast<Interface::I_move>(p);
    if (move_popper) {
      this->add_moving(move_popper);
    }
  });

  // 將塔加入容器
  tower->setPath(current_path);
  towers.push_back(tower);

  // 將塔的視覺元素加入渲染器
  if (auto body = tower->getBody()) {
    m_Renderer->AddChild(body);
  }
  if (auto range = tower->getRange()) {
    m_Renderer->AddChild(range);
  }
}

void Manager::handleTowers() {
  for (auto &tower : towers) {
    // 塔的碰撞組件
    auto towerCollider = tower->getCollisionComponent();
    if (!towerCollider)
      continue;

    // 收集在射程內的氣球和距離
    std::vector<std::shared_ptr<Bloon>> bloonsInRange;
    std::vector<float> distancesInRange;

    for (auto &holder : bloons) {
      auto bloon = holder->get_bloon();

      // 檢查氣球是否在塔的射程內
      bool collision = false;
      auto bloonCollider =
          std::dynamic_pointer_cast<Interface::I_collider>(bloon);
      if (bloonCollider) {
        collision = towerCollider->isCollide(*bloonCollider);
      }

      if (collision) {
        bloonsInRange.push_back(bloon);
        distancesInRange.push_back(holder->get_distance());
      }
    }

    // 讓塔處理射程內的氣球
    tower->handleBloonsInRange(bloonsInRange, distancesInRange);
  }
}

// 流程相關
void Manager::next_wave() {
  if (m_game_state == game_state::menu && current_waves == -1) {
    set_gap();
    current_waves = 0;
    bloons_gen_list = loader::load_bloons(current_waves);
  } else if (m_game_state == game_state::playing) {
    set_gap();
    current_waves++;
    bloons_gen_list = loader::load_bloons(current_waves);
  } else if (m_game_state == game_state::playing && current_waves == 50) {
    set_menu();
    current_waves = -1;
  } else {
    LOG_ERROR("MNGR  : Invalid game state");
    // throw std::runtime_error("Invalid game state or wrong waves");
  }

  if (current_waves >= 0) {
    auto textComponent = std::dynamic_pointer_cast<Util::Text>(m_waveText);
    if (textComponent) {
        textComponent->SetText(std::to_string(current_waves));
    }
}
  start_wave();
}

void Manager::start_wave() {
  if (m_game_state == game_state::gap &&
      (current_waves != -1 || current_waves <= 50)) {
    set_playing();
  } else {
    LOG_ERROR("MNGR  : Invalid game state");
    // throw std::runtime_error("Invalid game state or wrong waves");
  }
  // start generate bloons
}
void Manager::wave_check() {
  static int counter = 0;
  int bloonInterval = 0;
  if (m_game_state == game_state::gap || m_game_state == game_state::menu) {
    return;
  }
  if (bloons.size() == 0 && bloons_gen_list.size() == 0) {
    counter = 0;
    next_wave();
    return;
  }

  if (current_waves >= 0) {
    bloonInterval = 15 - current_waves;
    if (bloonInterval < 5) {
      bloonInterval = std::ceil(5 - current_waves / 20.0);
    }
    // 確保最小間隔
    bloonInterval = std::max(1, bloonInterval);
    bloonInterval *= 1;
  }

  // 檢查是否應該產生新氣球
  if (bloons_gen_list.size() > 0) {
    counter++;
    if (counter >= bloonInterval) {
      counter = 0;
      auto bloon_type = bloons_gen_list.back();
      bloons_gen_list.pop_back();
      add_bloon(bloon_type, 0,10+counter/10.0);
    }
  }
}

void Manager::update() {
  m_Renderer->Update();
  m_frame_count++;
}

// bloon_holder 內部類別實現
Manager::bloon_holder::bloon_holder(std::shared_ptr<Bloon> bloon,
                                    float distance,
                                    const std::shared_ptr<Path> path)
    : m_bloon(bloon), m_path(path), distance(distance) {}

float Manager::bloon_holder::get_distance() { return distance; }

Util::PTSDPosition Manager::bloon_holder::next_position(int frames = 1) {
  return m_path->getPositionAtDistance(m_bloon->GetSpeed() * frames + distance);
}

void Manager::bloon_holder::move() {
  if (m_bloon == nullptr)
    return;
  distance += m_bloon->GetSpeed();
  m_bloon->setPosition(next_position(0));
}

// 設定拖曳物件
void Manager::set_dragging(
    const std::shared_ptr<Interface::I_draggable> &draggable) {
  if (m_mouse_status == mouse_status::free) {
    LOG_DEBUG("MNGR  : dragging start");
    this->dragging = draggable;
    m_mouse_status = mouse_status::drag;
    // 通知物件開始拖曳
    if (dragging) {
      dragging->onDragStart();
    }
  }
}

// 結束拖曳狀態
void Manager::end_dragging() {
  if (m_mouse_status == mouse_status::drag) {
    LOG_DEBUG("MNGR  : dragging end");
    // 通知物件結束拖曳
    if (dragging) {
      dragging->onDragEnd();
    }
    dragging = nullptr;
    m_mouse_status = mouse_status::free;
  }
}

// 更新被拖曳的物件
void Manager::updateDraggingObject(const Util::PTSDPosition &cursor_position) {
  if (m_mouse_status == mouse_status::drag && dragging != nullptr) {
    dragging->onDrag(cursor_position);
  }
}

// 處理氣球狀態
void Manager::processBloonsState() {
  std::vector<std::shared_ptr<bloon_holder>> popped_bloons;

  for (auto &bloon : bloons) {
    if (bloon->get_bloon()->GetState() == Bloon::State::pop) {
      popped_bloons.push_back(bloon);
    }
  }

  // 在迴圈外處理爆炸，避免迭代器失效
  for (auto &bloon : popped_bloons) {
    pop_bloon(bloon);
  }
}

// 更新所有移動物件
void Manager::updateAllMovingObjects() {
  for (auto &move : movings) {
    move->move();
  }
}