#include "UI/buttons/tower_btn_conf.hpp"
#include "Util/Logger.hpp"
#include "conf.hpp"
#include "config.hpp"

namespace UI {

// 初始化靜態成員
std::unordered_map<Tower::TowerType, TowerButtonConfig>
    TowerButtonConfigManager::s_configs;

void TowerButtonConfigManager::Initialize() {
  // 清除現有配置
  s_configs.clear();

  // 添加預設塔配置
  s_configs.emplace(Tower::TowerType::dart,
                    TowerButtonConfig(Tower::TowerType::dart, "Dart Monkey",
                                      RESOURCE_DIR
                                      "/buttons/tower/dart_monkey.png",
                                      COST_DART, true));

  s_configs.emplace(
      Tower::TowerType::boomerang,
      TowerButtonConfig(Tower::TowerType::boomerang, "Boomerang Monkey",
                        RESOURCE_DIR "/buttons/tower/boomerang.png",
                        COST_BOOMERANG, true));

  s_configs.emplace(Tower::TowerType::spike,
                    TowerButtonConfig(Tower::TowerType::spike, "Spike",
                                      RESOURCE_DIR "/buttons/tower/spike.png",
                                      COST_SPIKES, true));

  s_configs.emplace(Tower::TowerType::tack,
                    TowerButtonConfig(Tower::TowerType::tack, "Tack Shooter",
                                      RESOURCE_DIR "/buttons/tower/tack.png",
                                      COST_TACK, true));

  s_configs.emplace(Tower::TowerType::ice,
                    TowerButtonConfig(Tower::TowerType::ice, "Ice Ball",
                                      RESOURCE_DIR
                                      "/buttons/tower/ice_ball.png",
                                      COST_ICE, true));

  s_configs.emplace(Tower::TowerType::bomb,
                    TowerButtonConfig(Tower::TowerType::bomb, "CANNON",
                                      RESOURCE_DIR "/buttons/tower/cannon.png",
                                      COST_BOMB, true));

  s_configs.emplace(Tower::TowerType::glue,
                    TowerButtonConfig(Tower::TowerType::glue, "Glue Gunner",
                                      RESOURCE_DIR "/buttons/tower/glue.png",
                                      COST_GLUE, true));

  s_configs.emplace(Tower::TowerType::super,
                    TowerButtonConfig(Tower::TowerType::super, "Super Monkey",
                                      RESOURCE_DIR
                                      "/buttons/tower/super_monkey.png",
                                      COST_SUPER, true));

  LOG_INFO("TOWER_BTN_CONF: 已初始化 {} 個塔按鈕配置", s_configs.size());
}

const TowerButtonConfig &
TowerButtonConfigManager::GetConfig(Tower::TowerType type) {
  if (s_configs.empty()) {
    Initialize();
  }

  auto it = s_configs.find(type);
  if (it == s_configs.end()) {
    LOG_ERROR("TOWER_BTN_CONF: 未找到類型為 {} 的塔按鈕配置",
              static_cast<int>(type));
    // 返回默認配置(使用dart作為默認)
    return s_configs.at(Tower::TowerType::dart);
  }

  return it->second;
}

std::vector<TowerButtonConfig>
TowerButtonConfigManager::GetAllAvailableConfigs() {
  if (s_configs.empty()) {
    Initialize();
  }

  std::vector<TowerButtonConfig> availableConfigs;

  for (const auto &pair : s_configs) {
    if (pair.second.available) {
      availableConfigs.push_back(pair.second);
    }
  }

  return availableConfigs;
}

void TowerButtonConfigManager::SetAvailability(Tower::TowerType type,
                                               bool available) {
  if (s_configs.empty()) {
    Initialize();
  }

  auto it = s_configs.find(type);
  if (it != s_configs.end()) {
    it->second.available = available;
    LOG_DEBUG("TOWER_BTN_CONF: 塔類型 {} 的可用性設置為 {}",
              static_cast<int>(type), available);
  }
}

} // namespace UI
