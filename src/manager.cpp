#include "manager.hpp"
#include "Util/Image.hpp"
#include "Util/Logger.hpp"
#include "Util/Position.hpp"
#include "Util/Renderer.hpp"
#include "bloon.hpp"
#include "move.hpp"
#include <algorithm>
#include <chrono>
#include <glm/fwd.hpp>
#include <memory>
#include <random>
#include <vector>

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
}

// 地圖管理相關函數
void Manager::add_map(const std::shared_ptr<Map> &map) {
  maps.push_back(map);
  m_Renderer->AddChild(map);
}

void Manager::set_map(int diff) {
  if (diff != 0 && diff != 1 && diff != 2) {
    LOG_ERROR("Invalid map diff");
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
void Manager::add_bloon(Bloon::Type type, float distance) {
  auto bloon = std::make_shared<Bloon>(
      type, current_path->getPositionAtDistance(distance));
  bloon->set_can_click(true); // 僅供測試
  this->add_click(bloon);

  register_mortal(bloon);

  auto bloon_holder =
      std::make_shared<Manager::bloon_holder>(bloon, distance, current_path);

  m_Renderer->AddChild(bloon);
  movings.push_back(bloon_holder);
  bloons.push_back(bloon_holder);
}

void Manager::pop_bloon(std::shared_ptr<bloon_holder> bloon) {
  bloon->get_bloon()->SetVisible(false);

  // 產生一個不重複的 1~4 順序
  std::vector<int> values = {1, 2, 3, 4};
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle(values.begin(), values.end(), std::default_random_engine(seed));

  // 處理子氣球
  auto sub_bloons = bloon->get_bloon()->GetChildBloons();
  for (size_t i = 0; i < sub_bloons.size() && i < values.size(); ++i) {
    float distance = bloon->get_distance() - values[i] * 5;
    this->add_bloon(*sub_bloons[i], distance);
  }

  bloon->get_bloon()->kill();
}

// 拖曳相關函數
void Manager::set_dragging(const std::shared_ptr<Collapsible> &dragging) {
  if (m_mouse_status == mouse_status::free) {
    LOG_INFO("start dragging");
    this->dragging = dragging;
    m_mouse_status = mouse_status::drag;
  }
}

/**
 * @brief 結束拖曳狀態
 */
void Manager::end_dragging() {
  if (m_mouse_status == mouse_status::drag) {
    LOG_INFO("ender dragoned");
    dragging = nullptr;
    m_mouse_status = mouse_status::free;
  }
}

// 更新被拖曳的物件
void Manager::updateDraggingObject(const Util::PTSDPosition &cursor_position) {
  if (m_mouse_status == mouse_status::drag && dragging != nullptr) {
    dragging->set_position(cursor_position);
  }
}

// 處理氣球狀態
void Manager::processBloonsState() {
  std::vector<std::shared_ptr<bloon_holder>> popped_bloons;

  for (auto &bloon : bloons) {
    if (bloon->get_bloon()->get_state() == Bloon::State::pop) {
      popped_bloons.push_back(bloon);
    }
  }

  // 在迴圈外處理爆炸，避免迭代器失效
  for (auto &bloon : popped_bloons) {
    pop_bloon(bloon);
  }

  // 清理已死亡的物件
}

// 更新所有移動物件
void Manager::updateAllMovingObjects() {
  for (auto &move : movings) {
    move->move();
  }
}

// 處理點擊事件
void Manager::handleClickAt(const Util::PTSDPosition &cursor_position) {
  static bool drag_cooldown = false; // 將 drag_cd 移入 Manager

  // 重置冷卻
  drag_cooldown = false;

  // 如果正在拖曳，則結束拖曳
  if (m_mouse_status == mouse_status::drag && !drag_cooldown) {
    end_dragging();
    drag_cooldown = true;
    return;
  }

  // 檢查是否點擊了可互動物件
  for (auto &clickable : clicks) {
    // 跳過不可點擊或已在冷卻的物件
    if (!(clickable->get_can_click() && !drag_cooldown)) {
      continue;
    }

    // 檢查點擊碰撞
    if (clickable->isCollide(cursor_position)) {
      // 處理可拖曳物件
      if (clickable->get_can_drag() && m_mouse_status == mouse_status::free &&
          !drag_cooldown) {
        set_dragging(clickable);
        drag_cooldown = true;
      }

      // 處理點擊事件
      if (clickable->get_can_click()) {
        clickable->be_clicked();
        LOG_INFO("Clicked");
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

  // 1. 清理 bloons 容器
  bloons.erase(std::remove_if(
                   bloons.begin(), bloons.end(),
                   [&dead_uuids](const auto &bloon) {
                     auto mortal =
                         std::dynamic_pointer_cast<Mortal>(bloon->get_bloon());
                     if (mortal) {
                       return std::find(dead_uuids.begin(), dead_uuids.end(),
                                        mortal->get_uuid()) != dead_uuids.end();
                     }
                     return false;
                   }),
               bloons.end());

  // 2. 清理 movings 容器
  movings.erase(
      std::remove_if(
          movings.begin(), movings.end(),
          [&dead_uuids](const auto &moving) {
            auto mortal = std::dynamic_pointer_cast<Mortal>(moving);
            if (mortal) {
              return std::find(dead_uuids.begin(), dead_uuids.end(),
                               mortal->get_uuid()) != dead_uuids.end();
            }

            // 特殊處理 bloon_holder 類型
            std::shared_ptr<bloon_holder> bloon_holder =
                std::dynamic_pointer_cast<Manager::bloon_holder>(moving);
            if (bloon_holder) {
              auto bloon =
                  std::dynamic_pointer_cast<Mortal>(bloon_holder->get_bloon());
              if (bloon) {
                return std::find(dead_uuids.begin(), dead_uuids.end(),
                                 bloon->get_uuid()) != dead_uuids.end();
              }
            }

            return false;
          }),
      movings.end());

  // 3. 清理 clicks 容器
  clicks.erase(std::remove_if(
                   clicks.begin(), clicks.end(),
                   [&dead_uuids](const auto &click) {
                     auto mortal = std::dynamic_pointer_cast<Mortal>(click);
                     if (mortal) {
                       return std::find(dead_uuids.begin(), dead_uuids.end(),
                                        mortal->get_uuid()) != dead_uuids.end();
                     }
                     return false;
                   }),
               clicks.end());
}


// bloon_holder 內部類別實現
Manager::bloon_holder::bloon_holder(std::shared_ptr<Bloon> bloon,
                                    float distance,
                                    const std::shared_ptr<Path> path)
    : m_bloon(bloon), m_path(path), distance(distance) {}

float Manager::bloon_holder::get_distance() { return distance; }

Util::PTSDPosition Manager::bloon_holder::next_position(int frames) {
  return m_path->getPositionAtDistance(m_bloon->GetSpeed() * frames + distance);
}

void Manager::bloon_holder::move() {
  distance += m_bloon->GetSpeed();
  m_bloon->set_position(m_path->getPositionAtDistance(distance));
}