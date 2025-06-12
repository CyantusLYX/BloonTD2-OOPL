#include "core/manager.hpp"
#include "UI/button.hpp"
#include "Util/Color.hpp"
#include "Util/GameObject.hpp"
#include "Util/Input.hpp"
#include "Util/Logger.hpp"
#include "Util/Position.hpp"
#include "Util/SFX.hpp"
#include "components/mortal.hpp"
#include "conf.hpp"
#include "core/ShapeAnimation.hpp"
#include "core/shape.hpp"
#include "entities/bloon.hpp"
#include "entities/poppers/glue.hpp"
#include <cstdlib>
#include <glm/fwd.hpp>
#include <memory>

bool toggle_show_collision_at = 0;
bool toggle_show_bloons = 0;
bool voltog = 1;
bool inf = 0;

// 座標轉換輔助函數
glm::vec2 to_pos(glm::vec2 vec) { // from vec2(sdl) to ptsd to vec2
  auto pos = Util::PTSDPosition::FromSDL(vec.x, vec.y);
  return pos.ToVec2();
}

// Manager 建構函數
Manager::Manager(std::shared_ptr<Util::Renderer> &renderer)
    : m_Renderer(renderer),
      life(100),
      money(STARTING_MONEY - 1) {

  // 初始化塔工廠
  initTowerFactories();

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
      {to_pos({74, -10}), to_pos({74, 192}), to_pos({160, 192}),
       to_pos({160, 292}), to_pos({64, 292}), to_pos({64, 394}),
       to_pos({246, 394}), to_pos({246, 90}), to_pos({427, 90}),
       to_pos({427, 190}), to_pos({334, 190}), to_pos({334, 293}),
       to_pos({418, 293}), to_pos({418, 502})},
      // 困難地圖路徑 (1)
      {to_pos({51, -10}), to_pos({51, 250}), to_pos({265, 250}),
       to_pos({265, 340}), to_pos({350, 340}), to_pos({350, 250}),
       to_pos({420, 250}), to_pos({420, 435}), to_pos({220, 435}),
       to_pos({220, 502})},
      // 困難地圖路徑 (2)
      {to_pos({296, -10}), to_pos({296, 75}), to_pos({400, 75}),
       to_pos({400, 185}), to_pos({235, 178}), to_pos({235, 75}),
       to_pos({150, 75}), to_pos({150, 320}), to_pos({74, 324}),
       to_pos({75, 400}), to_pos({140, 400}), to_pos({140, 502})}};

  // 創建並添加地圖
  for (size_t i = 0; i < map_paths.size(); ++i) {
    auto map =
        std::make_shared<Map>(std::make_shared<Util::Image>(map_paths[i]), 1,
                              std::make_shared<Path>(paths[i], 40), false);
    this->add_map(map);
  }

  // Store all paths for later use
  all_paths.clear();
  for (size_t i = 0; i < paths.size(); ++i) {
    all_paths.push_back(std::make_shared<Path>(paths[i], 40));
  }
  m_waveText->m_Transform.translation = Util::PTSDPosition(-280, -200).ToVec2();
  m_Renderer->AddChild(m_waveText);

  // sfx
  add_button(sound);
  add_clickable(sound);

  // start_round
  b_start_round->SetZIndex(100);
  add_button(b_start_round);
  add_clickable(b_start_round);
  add_button(end_game);
  add_clickable(end_game);

  infinity->setSize({60,25});
  add_button(infinity);
  add_clickable(infinity);
  clear->setSize({60,25});
  add_button(clear);
  add_clickable(clear);
  skip->setSize({60,25});
  add_button(skip);
  add_clickable(skip);

  // start_round animation
  std::vector<glm::vec2> a_sizes = std::vector<glm::vec2>{};
  std::vector<Util::Color> a_colors = std::vector<Util::Color>{};

  for (int i = 0; i < 16; i++) {
    a_sizes.push_back(b_start_round->GetScaledSize());
    a_colors.push_back(Util::Color(0, 255, 0, 255 / (i + 1)));
  }
  for (int i = 16; i > 0; i--) {
    a_sizes.push_back(b_start_round->GetScaledSize());
    a_colors.push_back(Util::Color(0, 255, 0, 255 / (i + 1)));
  }

  auto startround_shapeanim = std::make_shared<Util::ShapeAnimation>(
      Util::ShapeType::Rectangle, a_sizes, a_colors, true, 50, true, 500);

  startround_anim = std::make_shared<Util::GameObject>(
      startround_shapeanim, 50.1f, Util::PTSDPosition(-235, 180), true);
  
  add_object(startround_anim);

  ban = std::make_shared<Manager::banners>(
    std::make_shared<Util::Image>(RESOURCE_DIR "/titles/gg.png"), 50.f);
  ban->SetZIndex(50.1f);
  add_object(ban);
  // menu
  emh_menu_buttons.push_back(std::make_shared<Button>(
      "easy", Util::PTSDPosition(-200, 100), glm::vec2(50, 50)));
  emh_menu_buttons.push_back(std::make_shared<Button>(
      "med", Util::PTSDPosition(-50, 100), glm::vec2(50, 50)));
  emh_menu_buttons.push_back(std::make_shared<Button>(
      "hard", Util::PTSDPosition(100, 100), glm::vec2(50, 50)));
  for (auto btn : emh_menu_buttons) {
    add_button(btn);
    add_clickable(btn);
    emh_medals.push_back(std::make_shared<Util::GameObject>(
        std::make_shared<Util::Image>(RESOURCE_DIR "/buttons/medal_slot.png"),
        10));
    add_object(emh_medals.back());
  }
  for (int i = 0; i < 3; i++) {
    emh_medals[i]->m_Transform.translation =
        Util::PTSDPosition(-200 + i * 150, 40).ToVec2();
  }

  initUI();
}

void Manager::menu_hover(Util::PTSDPosition now) {
  int c = current_diff;
  for (int i = 0; i < 3; i++) {
    auto btn = emh_menu_buttons[i];
    if (btn->isCollide(now) && c != i) {
      set_map(i);
      //   btn->SetDrawable(
      //       std::make_shared<Util::Image>(RESOURCE_DIR
      //       "/buttons/Bplay.png"));
      // }
      // else{
      //   btn->SetDrawable(
      //     std::make_shared<Util::Image>(std::string(RESOURCE_DIR
      //     "/buttons/B") + btn->getName() + ".png"));
    }
  }
}

// 根據類型創建塔
std::shared_ptr<Tower::Tower>
Manager::createTower(Tower::TowerType type,
                     const Util::PTSDPosition &position) {
  // 檢查是否為塔類型
  if (!BuyableConfigManager::IsTower(type)) {
    LOG_ERROR("MNGR  : 試圖創建非塔類型 {}", static_cast<int>(type));
    return nullptr;
  }

  auto factoryIt = m_towerFactories.find(type);
  if (factoryIt != m_towerFactories.end() && factoryIt->second) {
    auto tower = factoryIt->second(position);
    if (tower) {
      // 從配置中載入塔屬性
      const auto &config = BuyableConfigManager::GetTowerConfig(type);
      tower->setRange(config.range);
      tower->setSpecConfig(config.specConfig);
      //  如果塔有其他屬性可從這裡設置，如攻擊速度等

      return tower;
    }
  }

  LOG_ERROR("MNGR  : 未找到塔類型 {} 的工廠函數", static_cast<int>(type));
  return nullptr;
}

std::shared_ptr<popper>
Manager::createPopper(Tower::TowerType type,
                      const Util::PTSDPosition &position) {
  // 檢查是否為 Popper 類型
  if (!BuyableConfigManager::IsPopper(type)) {
    LOG_ERROR("MNGR  : 試圖創建非 Popper 類型 {}", static_cast<int>(type));
    return nullptr;
  }

  auto factoryIt = m_popperFactories.find(type);
  if (factoryIt != m_popperFactories.end() && factoryIt->second) {
    auto popper = factoryIt->second(position);
    if (popper) {
      // 從配置中載入 popper 屬性
      const auto &config = BuyableConfigManager::GetPopperConfig(type);

      // 根據 popper 類型設置生命值
      if (type == Tower::TowerType::spike) {
        auto spikePtr = std::dynamic_pointer_cast<spike>(popper);
        if (spikePtr) {
          spikePtr->setLife(config.durability);
        }
      } else if (type == Tower::TowerType::glue) {
        auto gluePtr = std::dynamic_pointer_cast<Glue>(popper);
        if (gluePtr) {
          gluePtr->setLife(config.durability);
        }
      }

      return popper;
    }
  }

  LOG_ERROR("MNGR  : 未找到 Popper 類型 {} 的工廠函數", static_cast<int>(type));
  return nullptr;
}

// 開始拖曳一個新塔
void Manager::startDraggingTower(Tower::TowerType towerType) {
  // 檢查是否已經在拖曳狀態
  if (m_mouse_status != mouse_status::free) {
    LOG_DEBUG("MNGR  : 無法開始拖曳，當前不處於自由狀態");
    return;
  }

  // 獲取物品成本
  int cost = BuyableConfigManager::GetCost(towerType);

  // 檢查金錢是否足夠
  if (cost > money && !inf) {
    LOG_DEBUG("MNGR  : 金錢不足，無法購買物品");
    return;
  }

  // 獲取當前游標位置
  Util::PTSDPosition cursorPos = Util::Input::GetCursorPosition();

  // 創建可拖曳物件
  std::shared_ptr<Interface::I_draggable> draggable;

  if (BuyableConfigManager::IsTower(towerType)) {
    // 創建塔實例
    auto tower = createTower(towerType, cursorPos);
    if (tower) {
      // 設置為預覽模式
      tower->setPreviewMode(true);

      // 確保塔身和範圍是可見的
      if (tower->getBody()) {
        tower->getBody()->SetVisible(true);
        m_Renderer->AddChild(tower->getBody());
      }

      if (tower->getRange()) {
        tower->getRange()->setVisible(true);
        m_Renderer->AddChild(tower->getRange());
      }

      // 設置拖曳物件
      draggable = std::dynamic_pointer_cast<Interface::I_draggable>(tower);
    }
  } else if (BuyableConfigManager::IsPopper(towerType)) {
    // 創建 Popper 實例
    auto popper = createPopper(towerType, cursorPos);
    if (popper) {
      // 設置拖曳物件
      draggable = std::dynamic_pointer_cast<Interface::I_draggable>(popper);

      // 添加到渲染器
      m_Renderer->AddChild(popper->get_object());
    }
  }

  if (draggable) {
    // 設置拖曳狀態
    draggable->setDraggable(true);

    // 記錄拖曳信息（但不存儲塔物件引用）
    m_isTowerDragging = true;
    m_dragTowerType = towerType;
    m_dragTowerCost = cost;

    // 開始拖曳
    set_dragging(draggable);

    LOG_DEBUG("MNGR  : 開始拖曳一個新的 {} 物品",
              BuyableConfigManager::GetBaseConfig(towerType).name);
  } else {
    LOG_ERROR("MNGR  : 無法創建可拖曳物品");
  }
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

  // Set up paths based on difficulty
  current_paths.clear();
  if (diff == 2) {                         // Hard map has two paths
    current_paths.push_back(all_paths[2]); // First hard path
    current_paths.push_back(all_paths[3]); // Second hard path
    LOG_INFO("MNGR  : Hard map selected with 2 paths");
  } else {
    // Easy and medium maps use single path
    current_paths.push_back(maps[diff]->get_path());
    LOG_INFO("MNGR  : Map {} selected with 1 path", diff);
  }
}

std::shared_ptr<Map> Manager::get_curr_map() {
  return maps[current_diff];
}

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
void Manager::add_bloon(Bloon::Type type, float distance, float z_index) {
  // For hard map, randomly choose between the two paths
  int pathId = 0;
  if (current_diff == 2 && current_paths.size() > 1) {
    // Hard map: randomly choose path 0 or 1
    pathId = rand() % 2;
  }

  if (pathId >= static_cast<int>(current_paths.size()) ||
      !current_paths[pathId]) {
    LOG_ERROR("MNGR  : Invalid path ID or path not found");
    return;
  }

  auto bloon = std::make_shared<Bloon>(
      type, current_paths[pathId]->getPositionAtDistance(distance), z_index);
  bloon->setPathId(pathId);   // Set the path ID for the bloon
  bloon->setClickable(true);  // 更改為使用新介面方法
  this->add_clickable(bloon); // 使用新的方法

  register_mortal(bloon);

  auto bloon_holder =
      std::make_shared<Manager::bloon_holder>(bloon, distance, current_paths);

  m_Renderer->AddChild(bloon);
  movings.push_back(bloon_holder);
  bloons.push_back(bloon_holder);

  LOG_DEBUG("MNGR  : Added bloon on path {} of {}", pathId,
            current_paths.size());
}

// Overloaded version that accepts path_id
void Manager::add_bloon(Bloon::Type type, float distance, int path_id,
                        float z_index) {
  if (path_id >= static_cast<int>(current_paths.size()) ||
      !current_paths[path_id]) {
    LOG_ERROR("MNGR  : Invalid path ID or path not found");
    return;
  }

  auto bloon = std::make_shared<Bloon>(
      type, current_paths[path_id]->getPositionAtDistance(distance), z_index);
  bloon->setPathId(path_id);  // Set the path ID for the bloon
  bloon->setClickable(true);  // 更改為使用新介面方法
  this->add_clickable(bloon); // 使用新的方法

  register_mortal(bloon);

  auto bloon_holder =
      std::make_shared<Manager::bloon_holder>(bloon, distance, current_paths);

  m_Renderer->AddChild(bloon);
  movings.push_back(bloon_holder);
  bloons.push_back(bloon_holder);
}

void Manager::add_button(const std::shared_ptr<Button> &button) {
  buttons.push_back(button);
  if (button->isDrawable)
    m_Renderer->AddChild(button); // 將按鈕加入渲染器
  register_mortal(button);
}

void Manager::pop_bloon(std::shared_ptr<bloon_holder> bloon, bool fx) {

  if (bloon->get_bloon()->getPosition().ToVec2().y >
      get_curr_map()->get_path()->getPositionAtPercentage(.99).ToVec2().y) {
    money++;
    if (fx) {
      auto popimg_tmpobj = std::make_shared<popimg_class>();
      popimg_tmpobj->pop_n_return_img(bloon->get_bloon()->getPosition());
      register_mortal(popimg_tmpobj);
      popimgs.push_back(popimg_tmpobj);
      m_Renderer->AddChild(popimg_tmpobj);
      m_Renderer->RemoveChild(popimgs.back()->getobj());
    }
    bloon->get_bloon()->SetVisible(false);
  }
  //}else life--;

  // 產生一個不重複的 1~4 順序
  std::vector<int> values = {1, 2, 3, 4};
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle(values.begin(), values.end(), std::default_random_engine(seed));

  // 處理子氣球
  auto sub_bloons = bloon->get_bloon()->GetChildBloons();
  for (size_t i = 0; i < sub_bloons.size() && i < values.size(); ++i) {
    float distance = bloon->get_distance() - values[i] * 5;
    if (toggle_show_bloons)
      LOG_DEBUG(
          "MNGR  : Gen bloon {} at distance {}",
          std::string(magic_enum::enum_name(bloon->get_bloon()->GetType())),
          distance);
    // Use the same path as the parent bloon for child bloons
    int parent_path_id = bloon->get_bloon()->getPathId();
    this->add_bloon(*sub_bloons[i], distance, parent_path_id,
                    11 + frame_count / 10.0f);
  }

  bloon->kill();
  // m_Renderer->RemoveChild(std::shared_ptr<Util::GameObject>
  // (bloon->get_bloon()));
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

void Manager::menu_control(int diff) {
  set_map(diff);
  menu_control(false);
  m_game_state = game_state::gap;
}

void Manager::menu_control(bool visible) {
  for (auto btn : emh_menu_buttons) {
    btn->SetVisible(visible);
    btn->setClickable(visible);
  }
  for (auto medal : emh_medals) {
    medal->SetVisible(visible);
  }
}

void Manager::handleClickAt(const Util::PTSDPosition &cursor_position) {
  bool isTowerClicked = false;
  bool isUIElementClicked = false; // 新增：追蹤是否點擊了UI元素
  if (drag_cd) {
    LOG_DEBUG("MNGR  : 點擊冷卻中，忽略此次點擊");
    return;
  }
  // 如果正在拖曳，則結束拖曳 (第二次點擊時)
  if (m_mouse_status == mouse_status::drag) {
    if (dragging) {
      // 更新最後位置為點擊位置
      dragging->onDrag(cursor_position);

      // 如果是塔拖曳，則放置塔
      if (m_isTowerDragging) {
        placeCurrentTower(cursor_position);
      } else {
        // 其他拖曳物體（如果不需要特殊處理）
        end_dragging();
      }
    }
    return;
  }

  // 檢查是否點擊了可互動物件
  for (auto &clickable : clickables) {
    // 跳過不可點擊或已在冷卻的物件
    if (!(clickable->isClickable() && !drag_cd)) {
      continue;
    }

    if (toggle_show_collision_at)
      LOG_DEBUG("MNGR  : clickable");
    // 獲取具有碰撞功能的 GameObject
    /* auto gameObject =
       std::dynamic_pointer_cast<Util::GameObject>(clickable); if
       (!gameObject) continue; */

    // 檢查物件是否能夠進行碰撞檢測
    bool isCollided = false;

    // 透過介面來檢查碰撞
    auto collidable =
        std::dynamic_pointer_cast<Interface::I_collider>(clickable);
    if (collidable) {
      isCollided = collidable->isCollide(cursor_position);
    }

    if (isCollided || clickable->isClick(cursor_position)) {
      LOG_DEBUG("MNGR  : 檢測到點擊");

      // 1. 首先嘗試處理特殊類型按鈕 - TowerButton
      auto towerButton = std::dynamic_pointer_cast<TowerButton>(clickable);
      if (towerButton && (m_game_state == game_state::playing ||
                          m_game_state == game_state::gap)) {
        LOG_DEBUG("MNGR  : 檢測到塔按鈕點擊，類型: {}",
                  static_cast<int>(towerButton->getTowerType()));

        // 取消所有tower的preview模式並移除flag
        unSelectFlag();
        for (auto tower : towers) {
          if (tower->isPreviewMode()) {
            tower->setPreviewMode(false, false);
          }
        }

        // 觸發點擊事件
        clickable->onClick();

        // 塔按鈕相關操作 (留空，稍後實現)
        startDraggingTower(towerButton->getTowerType());

        isUIElementClicked = true;
        break;
      }

      // 2. 處理Tower點擊
      auto clickedTower = std::dynamic_pointer_cast<Tower::Tower>(clickable);
      if (clickedTower) {
        isTowerClicked = true;

        // 首先取消所有其他tower的preview模式並移除當前flag
        unSelectFlag();
        for (auto tower : towers) {
          if (tower != clickedTower && tower->isPreviewMode()) {
            tower->setPreviewMode(false, false);
          }
        }

        // 設置當前tower的preview模式
        clickedTower->setPreviewMode(true, false);

        // 創建並設置新的upgrades panel
        auto newUpgradesPanel =
            std::make_shared<UI::UpgradesPanel>(clickedTower, money);
        for (auto button : newUpgradesPanel->getButtons()) {
          add_button(button);
          add_clickable(button);
        }
        add_updatable(newUpgradesPanel);
        set_flag(newUpgradesPanel);

        isUIElementClicked = true;
        break;
      }
      // 3. 然後處理普通按鈕
      auto button = std::dynamic_pointer_cast<Button>(clickable);
      if (button) {
        isUIElementClicked = true; // 標記UI元素被點擊

        // 對於大部分按鈕，取消tower選擇（除了upgrade相關的按鈕）
        const std::string &buttonName = button->getName();
        if (buttonName != "upgrade" && buttonName != "sell" &&
            buttonName != "back") {
          unSelectFlag();
          for (auto tower : towers) {
            if (tower->isPreviewMode()) {
              tower->setPreviewMode(false, false);
            }
          }
        }

        // 觸發點擊事件
        clickable->onClick();

        // 根據按鈕名稱執行對應操作
        LOG_DEBUG("MNGR  : 檢測到普通按鈕點擊：{}", buttonName);

        if (buttonName == "start") {
          // 開始按鈕功能
          next_wave();
        } else if (buttonName == "end_game") {
          // 選單按鈕功能
          // 移除所有氣球
          for (auto &holder : bloons) {
            holder->kill();
          }
          for (auto &tower : towers) {
            tower->kill();
          }
          for (auto &popper : poppers) {
            popper->kill();
          }
          m_game_state = game_state::menu;
          menu_control(true);
        } else if (buttonName == "sound") {
          // 音效按鈕功能
          voltog = !voltog;
          for (auto a : popimgs) {
            a->voltoggle(voltog);
          }
          button->SetDrawable(std::make_shared<Util::Image>(
              voltog ? RESOURCE_DIR "/buttons/Bsound.png"
                     : RESOURCE_DIR "/buttons/Bmute.png"));
        } else if (buttonName == "easy") {
          set_map(0);
          createSpikeAtEnd(); // 在正確的地圖設置後創建終點釘子
          money= STARTING_MONEY; // 重置金錢
          life = MAX_LIVES_EASY;
          for (auto btn : emh_menu_buttons) {
            btn->SetVisible(false);
            btn->setClickable(false);
          }
          set_playing();
          menu_control(0);
        } else if (buttonName == "med") {
          set_map(1);
          createSpikeAtEnd(); // 在正確的地圖設置後創建終點釘子
          money= STARTING_MONEY; // 重置金錢
          life = MAX_LIVES_MEDIUM;
          for (auto btn : emh_menu_buttons) {
            btn->SetVisible(false);
            btn->setClickable(false);
          }
          set_playing();
          menu_control(1);
        } else if (buttonName == "hard") {
          set_map(2);
          createSpikeAtEnd(); // 在正確的地圖設置後創建終點釘子
          money= STARTING_MONEY; // 重置金錢
          life = MAX_LIVES_HARD;
          for (auto btn : emh_menu_buttons) {
            btn->SetVisible(false);
            btn->setClickable(false);
          }
          set_playing();
          menu_control(2);
        } else if (buttonName == "sell") {
          // 賣出按鈕功能
          auto sellButton = std::dynamic_pointer_cast<UI::SellButton>(button);
          if (sellButton) {
            sellButton->onClick();
          }
          money += sellButton->getSellPrice();
          sellButton->getTower()->kill(); // 賣出塔
          unSelectFlag();
        } else if (buttonName == "upgrade") {
          // 升級按鈕功能
          auto upgradeButton =
              std::dynamic_pointer_cast<UI::UpgradeButton>(button);
          if (upgradeButton) {
            upgradeButton->onClick();
            money -= upgradeButton->getCost();
          }
        } else if (buttonName == "cancel") {
          // 取消按鈕功能
          cancelTowerPlacement();
        } else if (buttonName == "back") {
          // 返回按鈕功能
          unSelectFlag();
        } else if (buttonName == "start_round") {
          // 開始新一輪
          if (m_game_state == game_state::gap) {
            m_game_state = game_state::playing;
            next_wave();
            f_wave_end = 0;
          } else if (m_game_state == game_state::playing) {
            LOG_DEBUG("MNGR  : 已經在進行中，無法再次開始");
          }
        }else if(buttonName == "skip"){
          if(current_waves<40) current_waves += 10;//skip 10 level
          else {
            m_game_state = game_state::over;
            over = 1;
          }
        } else if(buttonName == "clear"){
          for (auto &holder : bloons) {
            m_Renderer->RemoveChild(holder->get_bloon());
          }
          m_game_state = game_state::gap;
        }else if(buttonName == "infinity"){
          inf = !inf;
          button->SetDrawable(std::make_shared<Util::Image>(
              inf ? RESOURCE_DIR "/buttons/Binfinity.png"
                  : RESOURCE_DIR "/buttons/Binfinity_n.png"));
        } else if (buttonName == "voltoggle") {
          voltog = !voltog;
          button->SetDrawable(std::make_shared<Util::Image>(
              voltog ? RESOURCE_DIR "/buttons/Bsound.png"
                     : RESOURCE_DIR "/buttons/Bmute.png"));
        }
        else {
          LOG_ERROR("MNGR  : 未知按鈕名稱或按鈕目前無法使用: {}", buttonName);

          break;
        }
      }

      // 處理可能的拖曳狀態
      auto draggable =
          std::dynamic_pointer_cast<Interface::I_draggable>(clickable);
      if (draggable && draggable->isDraggable() &&
          m_mouse_status == mouse_status::free && !drag_cd) {
        set_dragging(draggable);
        drag_cd = true;
        isUIElementClicked = true;
      }

      // 4. 最後處理其他可點擊物件
      if (!isUIElementClicked) {
        isUIElementClicked = true; // 標記有其他可點擊物件被點擊
      }

      drag_cd = true;

      break;
    }
  }

  // 如果沒有點擊到任何塔或UI元素，取消所有selections
  if (!isTowerClicked && !isUIElementClicked) {
    unSelectFlag();
    for (auto tower : towers) {
      if (tower->isPreviewMode()) {
        tower->setPreviewMode(false, false);
      }
    }
  }
}

// 處理 popper 物件
void Manager::handlePoppers() {
  for (auto &popper : poppers) {
    // 檢查 popper 是否存活且在任何路徑上
    bool onPath = false;
    for (const auto &path : current_paths) {
      if (path && path->isOnPath(popper->get_position())) {
        onPath = true;
        break;
      }
    }

    if (popper->is_alive() && onPath) {
      // 收集與 popper 碰撞的氣球
      std::vector<std::shared_ptr<Bloon>> collided_bloons;
      std::vector<std::shared_ptr<bloon_holder>> collided_holders;

      // 檢查所有氣球是否與 popper 碰撞
      for (auto &holder : bloons) {
        auto bloon = holder->get_bloon();
        if (toggle_show_collision_at)
          LOG_DEBUG("MNGR  : Checking collision with bloon at "
                    "({},{}) , ({},{})",
                    bloon->getPosition().x, bloon->getPosition().y,
                    popper->getPosition().x, popper->getPosition().y);
        // 使用氣球的碰撞檢測與 popper 的位置進行碰撞檢測
        auto popperCollider =
            std::dynamic_pointer_cast<Interface::I_collider>(popper);
        if (bloon->isCollide(popperCollider)) {
          collided_bloons.push_back(bloon);
          collided_holders.push_back(holder);
          if (std::dynamic_pointer_cast<end_spike>(popper)) {
            if(!inf) life--;
            if (life < 0) {
              over = 0;
              m_game_state = game_state::over;
            }
            // this->add_clickable(bloon); // 使用新的方法

            // bloon->kill();

            // auto bloon_holder =
            //     std::make_shared<Manager::bloon_holder>(bloon,
            //     distance, current_path);

            // m_Renderer->RemoveChild(bloon);
            // movings.push_back(bloon_holder);
            // bloons.push_back(bloon_holder);
          }
        }
      }

      // 如果有碰撞的氣球，調用 hit() 函數
      if (!collided_bloons.empty()) {
        auto hit_results = popper->hit(collided_bloons);
        // 處理被擊中的氣球
        for (size_t i = 0; i < hit_results.size(); ++i) {
          if (!hit_results[i]) {
            continue; // Skip bloons that weren't hit
          }

          auto &bloon = collided_bloons[i];
          bool can_pop = true;

          // 檢查是否為鉛氣球 - 只有爆炸型 popper 可以擊破
          if (bloon->GetType() == Bloon::Type::lead &&
              !popper->is_explosive()) {
            can_pop = false;
          }
          // 檢查是否為冰凍氣球 - 需要特殊 popper 或爆炸型
          else if (bloon->GetState() == Bloon::State::frozed &&
                   !(popper->getCanPopFrozen() || popper->is_explosive())) {
            can_pop = false;
          }
          // 檢查是否為黑色氣球 -
          // 黑色氣球免疫爆炸性武器，除非該爆炸武器有特殊增強
          else if (bloon->GetType() == Bloon::Type::black &&
                   popper->is_explosive() && !popper->getCanPopBlack()) {
            can_pop = false;
          }

          if (can_pop) {
            // Explosives pop without visual effects
            pop_bloon(collided_holders[i], !popper->is_explosive());
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
  for (auto child : mortal->get_children()) {
    if (auto child_mortal = std::dynamic_pointer_cast<Mortal>(child)) {
      mortals.push_back(child_mortal);
    }
  }
}

void Manager::cleanup_dead_objects() {
  // 首先找出所有已死亡的物件的 UUID
  std::vector<std::string> dead_uuids;
  for (const auto &mortal : mortals) {
    if (mortal->is_dead()) {
      dead_uuids.push_back(mortal->get_uuid());
      auto gameObject = std::dynamic_pointer_cast<Util::GameObject>(mortal);
      if (gameObject) {
        m_Renderer->RemoveChild(gameObject);
        gameObject = nullptr;
      }
      // 特別處理 popper 類型
      else {
        auto popperObj = std::dynamic_pointer_cast<popper>(mortal);
        if (popperObj && popperObj->get_object()) {
          m_Renderer->RemoveChild(popperObj->get_object());
        }
      }
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

  cleanup_container(buttons, [](const auto &button) {
    return std::dynamic_pointer_cast<Mortal>(button);
  });

  cleanup_container(bloons, [](const auto &bloon) {
    return std::dynamic_pointer_cast<Mortal>(bloon->get_bloon());
  });

  cleanup_container(movings, [](const auto &moving) {
    return std::dynamic_pointer_cast<Mortal>(moving);
  });

  cleanup_container(clickables, [](const auto &clickable) {
    return std::dynamic_pointer_cast<Mortal>(clickable);
  });

  cleanup_container(popimgs, [](const auto &popimg_class) {
    return std::dynamic_pointer_cast<Mortal>(popimg_class);
  });
  cleanup_container(towers, [](const auto &tower) {
    return std::dynamic_pointer_cast<Mortal>(tower);
  });
  cleanup_container(updatables, [](const auto &updatable) {
    return std::dynamic_pointer_cast<Mortal>(updatable);
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
  tower->setPaths(current_paths);
  towers.push_back(tower);

  // 將塔的視覺元素加入渲染器
  if (auto body = tower->getBody()) {
    m_Renderer->AddChild(body);
    mortals.push_back(body);
  }
  if (auto range = tower->getRange()) {
    m_Renderer->AddChild(range);
    mortals.push_back(range);
  }
  if (auto clickable =
          std::dynamic_pointer_cast<Interface::I_clickable>(tower)) {
    this->add_clickable(clickable);
  }
  mortals.push_back(tower); // 註冊為可死亡物件
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
    LOG_DEBUG("MNGR  : nwave psgap");
    current_waves++;
    bloons_gen_list = loader::load_bloons(current_waves);
    for (int _ = 0; _ < 50; _++) {
      // bloons_gen_list.push_back(Bloon::Type::rainbow);
    };
  } else if (m_game_state == game_state::playing && current_waves == 50) {
    set_menu();
    current_waves = -1;
  } else {
    LOG_ERROR("MNGR  : Invalid game state");
    // throw std::runtime_error("Invalid game state or wrong waves");
  }

  // current_waves+=50;
  if (current_waves >= 0 && current_waves <= 50) {
    LOG_INFO("MNGR  : new wave loaded");
    if (1) {
      try {
        m_waveText_text->SetText(std::to_string(current_waves + 1));
        m_waveText->SetDrawable(m_waveText_text);
      } catch (std::exception &e) { // exception should be caught by reference
        LOG_CRITICAL("exception: {}", e.what());
      }
      // LOG_DEBUG("NONSTD: into textComponent-inner
      // (manager.next_wave())");
    } else
      LOG_DEBUG("NONSTD: into textComponent-inner-else (manager.next_wave())");
  }

  // if (current_waves > 2) {
  //   for(auto obj : m_Renderer->get_children()){
  //     m_Renderer->RemoveChild(obj);
  //   }
  //   m_Renderer->AddChild(m_gameover);
  //   // m_game_state = game_state::over;
  // } else
  start_wave();
}

void Manager::start_wave() {
  if (m_game_state == game_state::gap &&
      (current_waves != -1 && current_waves <= 50)) {
    set_playing();
  } else if(current_waves > 50) {
    over = 1;
    m_game_state = game_state::over;
  }
  else {
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
  if (bloons.size() == 0 && bloons_gen_list.size() == 0 &&
      m_game_state == game_state::playing && !f_wave_end) {
    counter = 0;
    f_wave_end = true;
    m_waveText_text->SetText(std::to_string(current_waves + 2));
    return;
  }
  if (f_wave_end)
    set_gap();

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
      add_bloon(bloon_type, 0, static_cast<float>(10 + counter / 10.0));
    }
  }
}

void Manager::update() {
  m_Renderer->Update();
  m_frame_count++;
  updateUI();
  static int cooldown_frames = 0;
  if (cooldown_frames > 0) {
    cooldown_frames--;
  } else {
    // 全局變數 drag_cooldown，在 handleClickAt 中使用
    drag_cd = false;
  }
  for (auto updatable : updatables) {
    auto mortal = std::dynamic_pointer_cast<Mortal>(updatable); // 確保是 Mortal
    if (mortal->is_alive())
      updatable->update();
  }

  if (m_game_state == game_state::gap)
    startround_anim->SetVisible(true);
  else
    startround_anim->SetVisible(false);
}

// bloon_holder 內部類別實現
Manager::bloon_holder::bloon_holder(std::shared_ptr<Bloon> bloon,
                                    float distance,
                                    std::vector<std::shared_ptr<Path>> &paths)
    : m_bloon(bloon),
      m_paths(paths),
      distance(distance) {}

float Manager::bloon_holder::get_distance() {
  return distance;
}

Util::PTSDPosition Manager::bloon_holder::next_position(int frames = 1) {
  int pathId = m_bloon->getPathId();
  if (pathId < static_cast<int>(m_paths.size()) && m_paths[pathId]) {
    return m_paths[pathId]->getPositionAtDistance(m_bloon->GetSpeed() * frames +
                                                  distance);
  }
  return m_bloon->getPosition(); // fallback to current position
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
  if (m_mouse_status == mouse_status::drag && dragging) {
    // 調用拖曳結束處理
    dragging->onDragEnd();
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

  for (auto &bloonWrapper : bloons) {
    auto bloon = bloonWrapper->get_bloon();
    if (!bloon)
      continue;
    if (bloon->GetState() == Bloon::State::pop) {
      popped_bloons.push_back(bloonWrapper);
    } else if (bloon->GetState() == Bloon::State::frozed) {
      bloon->updateFreeze();
    }
  }

  int n = 0;
  for (auto &bloon : popped_bloons) {
    LOG_DEBUG("pop {}", n++);
    pop_bloon(bloon);
  }
}

// 更新所有移動物件
void Manager::updateAllMovingObjects() {
  for (auto &move : movings) {
    move->move();
  }
}

void Manager::placeCurrentTower(const Util::PTSDPosition &position) {
  if (!m_isTowerDragging || m_mouse_status != mouse_status::drag || !dragging) {
    return;
  }

  // 保存當前拖曳信息
  Tower::TowerType currentType = m_dragTowerType;
  int currentCost = m_dragTowerCost;

  // 檢查位置是否有效
  bool isValidPosition = true;

  // ... 路徑檢查代碼 ...

  if (!isValidPosition) {
    LOG_DEBUG("MNGR  : 無法在此處放置物品");
    return;
  }

  // 清除拖曳狀態
  auto oldDragging = dragging;
  dragging = nullptr;
  m_mouse_status = mouse_status::free;
  m_isTowerDragging = false;
  // 確保只扣一次錢
  if ((currentCost > 0 && money >= currentCost) || inf) {
    if(!inf) money -= currentCost;

    if (BuyableConfigManager::IsTower(currentType)) {
      // 嘗試將 dragging 轉換為 Tower
      auto tower = std::dynamic_pointer_cast<Tower::Tower>(oldDragging);
      if (tower) {
        // 將 dragging 中的塔設為非預覽模式
        tower->setPreviewMode(false);

        // 更新位置確保精確
        tower->setPosition(position);

        // 使用 add_tower 函數來正確設置塔
        add_tower(tower);

        tower->setDraggable(false);

        LOG_DEBUG("MNGR  : 使用拖曳中的塔放置，位置: ({}, {})", position.x,
                  position.y);
      }
    } else if (BuyableConfigManager::IsPopper(currentType)) {
      // 嘗試將 dragging 轉換為 popper
      auto npopper = std::dynamic_pointer_cast<popper>(oldDragging);
      if (npopper) {
        npopper->setPosition(position);
        add_popper(npopper);
      }
    }

    // 更新 UI
    updateUI();

    LOG_DEBUG("MNGR  : 成功放置一個 {}",
              BuyableConfigManager::GetBaseConfig(currentType).name);
  } else {
    LOG_ERROR("MNGR  : 金錢不足或費用錯誤 (cost: {}, money: {})", currentCost,
              money);
  }

  // 結束拖曳狀態（這個會調用 dragging->onDragEnd()）
  end_dragging();
  m_isTowerDragging = false;
}

void Manager::cancelTowerPlacement() {
  if (m_isTowerDragging) {
    LOG_DEBUG("MNGR  : 取消塔放置");

    // 如果 dragging 是塔，從渲染器中移除其視覺元素
    auto tower = std::dynamic_pointer_cast<Tower::Tower>(dragging);
    if (tower) {
      if (tower->getBody())
        m_Renderer->RemoveChild(tower->getBody());
      if (tower->getRange())
        m_Renderer->RemoveChild(tower->getRange());
    }

    // 結束拖曳狀態
    end_dragging();
    m_isTowerDragging = false;
  }
}

void Manager::updateUI() {
  // 檢查側邊欄是否存在
  if (m_sidebarManager) {
    // 更新金錢和生命值顯示
    m_sidebarManager->updateMoney(money);
    m_sidebarManager->updateLives(life);

    // 檢查塔按鈕狀態
    auto buttons = m_sidebarManager->getAllTowerButtons();

    // 更新按鈕可點擊狀態（根據金錢是否足夠）
    for (const auto &button : buttons) {
      // 檢查金錢是否足夠
      int cost = button->getCost();
      if(!inf) button->setClickable(money >= cost);
      else button->setClickable(true);
    }
  }
}

void Manager::initUI() {
  // 獲取窗口大小
  float screenWidth = 640;
  float screenHeight = 480;

  // 創建側邊欄 (位於右側)
  float sidebarWidth = 170.0f;
  float sidebarX = screenWidth / 2 - sidebarWidth / 2;

  // 創建側邊欄管理器
  m_sidebarManager = std::make_shared<UI::SidebarManager>(
      Util::PTSDPosition(sidebarX, 0.0f), screenHeight, sidebarWidth, 12.0f);

  // 設置渲染器引用
  m_sidebarManager->setRenderer(m_Renderer);

  LOG_INFO("MNGR  : 側邊欄創建於位置 ({}, {}), 寬度: {}", sidebarX, 0,
           sidebarWidth);

  // 初始化遊戲狀態
  m_sidebarManager->updateMoney(money);
  m_sidebarManager->updateLives(life);

  // 初始化塔按鈕配置
  UI::TowerButtonConfigManager::Initialize();

  // 初始化升級按鈕配置
  UI::UpgradeButtonConfigManager::Initialize();

  // 從配置中添加所有可用的塔按鈕
  auto towerConfigs = UI::TowerButtonConfigManager::GetAllAvailableConfigs();
  for (const auto &config : towerConfigs) {
    auto towerBtn = std::make_shared<TowerButton>(
        config.name,              // 名稱
        Util::PTSDPosition(0, 0), // 位置將由面板自動調整
        60.0f,                    // 按鈕大小 (這裡做成圓形)
        config.imagePath,
        money >= config.cost, // 是否可點擊取決於錢是否足夠
        config.type,          // 塔類型
        config.cost           // 成本
    );

    // 添加到側邊欄
    m_sidebarManager->addTowerButton(towerBtn);

    LOG_INFO("MNGR  : 已添加塔按鈕 {} (類型: {}, 成本: {})", config.name,
             static_cast<int>(config.type), config.cost);
  }

  // 獲取所有按鈕並添加到可點擊列表
  for (auto &btn : m_sidebarManager->getAllTowerButtons()) {
    clickables.push_back(btn);
    LOG_INFO("MNGR  : 已添加按鈕 {} 到可點擊列表", btn->getName());
  }
}

void Manager::popimg_tick_manager() {
  int max_tick = 3;
  for (auto &pimg : popimgs) {
    if (pimg->get_tick() > max_tick) {
      m_Renderer->RemoveChild(pimg);
      pimg->kill();
      // LOG_DEBUG("delete a popimg at its {}", pimg->get_tick());
    } else
      pimg->tick_add();
    // LOG_DEBUG("{} popimg", popimgs.size());
  }
  // LOG_DEBUG("pm");
};

void Manager::medal_setter(int diff) {
  static bool win[3] = {false, false, false};
  if (diff < 0 || diff > 2) {
    LOG_ERROR("MNGR  : Invalid difficulty level for medal setter");
    return;
  }

  if (win[diff]) {
    LOG_DEBUG("MNGR  : 已經獲得過此難度的獎牌，無需重複設置");
    return;
  }

  // 根據難度設置獎牌
  if (diff == 0) {
    emh_medals[0]->SetDrawable(
        std::make_shared<Util::Image>(RESOURCE_DIR "/buttons/Bmedal_e.png"));
  } else if (diff == 1) {
    emh_medals[1]->SetDrawable(
        std::make_shared<Util::Image>(RESOURCE_DIR "/buttons/Bmedal_m.png"));
  } else if (diff == 2) {
    emh_medals[2]->SetDrawable(
        std::make_shared<Util::Image>(RESOURCE_DIR "/buttons/Bmedal_h.png"));
  }

  LOG_INFO("MNGR  : 獎牌 {} 已設置為 1", diff);
}

// 在路徑終點生成終極釘子
void Manager::createSpikeAtEnd() {
  if (!current_map || current_paths.empty()) {
    LOG_ERROR("MNGR  : 無法創建終點釘子 - 地圖或路徑未設置");
    return;
  }

  // Create spikes at the end of all current paths
  for (size_t i = 0; i < current_paths.size(); ++i) {
    if (!current_paths[i])
      continue;

    auto pos_shift = Util::PTSDPosition(0, -5).ToVec2() +
                     current_paths[i]->getPositionAtPercentage(1).ToVec2();
    auto spike_at_end = std::make_shared<Manager::end_spike>(
        Util::PTSDPosition(pos_shift.x, pos_shift.y));
    spike_at_end->setLife(10000000);
    spike_at_end->setCanPopBlack(true);
    spike_at_end->setCanPopFrozen(true);
    spike_at_end->setExplosive(true);
    add_popper(spike_at_end);

    LOG_DEBUG("MNGR  : 在路徑 {} 終點創建終極釘子，位置: ({}, {})", i,
              pos_shift.x, pos_shift.y);
  }
}

void Manager::unSelectFlag() {
  if (current_flag) {
    current_flag->kill();
    set_flag(nullptr);
  }
}
