#include "UI/buttons/upgrade_btn_conf.hpp"
#include "Util/Logger.hpp"
#include "conf.hpp"

namespace UI {

// 初始化靜態成員
std::unordered_map<Tower::TowerType, TowerUpgradeConfig>
    UpgradeButtonConfigManager::s_configs;

void UpgradeButtonConfigManager::Initialize() {
  // 清除現有配置
  s_configs.clear();

  // Dart Monkey 升級配置
  s_configs.emplace(
      Tower::TowerType::dart,
      TowerUpgradeConfig(Tower::TowerType::dart,
                         UpgradeButtonConfig(
                             {"Long", "Range", "Darts"},
                             RESOURCE_DIR "/buttons/upgrades/dart_upgrade1.png",
                             COST_DART_RANGE_UPGRADE, true),
                         UpgradeButtonConfig(
                             {"Piercing", "Darts"},
                             RESOURCE_DIR "/buttons/upgrades/dart_upgrade2.png",
                             COST_DART_PIERCE_UPGRADE, true)));

  // Boomerang Monkey 升級配置
  s_configs.emplace(
      Tower::TowerType::boomerang,
      TowerUpgradeConfig(Tower::TowerType::boomerang,
                         UpgradeButtonConfig(
                             {"Multi", "Target"},
                             RESOURCE_DIR "/buttons/upgrades/dart_upgrade1.png",
                             COST_BOOMERANG_PIERCE_UPGRADE, true),
                         UpgradeButtonConfig(
                             {"Sonic", "Boom"},
                             RESOURCE_DIR "/buttons/upgrades/dart_upgrade2.png",
                             COST_BOOMERANG_SONIC_UPGRADE, true)));

  // Tack Shooter 升級配置
  s_configs.emplace(
      Tower::TowerType::tack,
      TowerUpgradeConfig(Tower::TowerType::tack,
                         UpgradeButtonConfig(
                             {"Faster", "Shooting"},
                             RESOURCE_DIR "/buttons/upgrades/dart_upgrade1.png",
                             COST_TACK_RATE_UPGRADE, true),
                         UpgradeButtonConfig(
                             {"Extra", "Range", "Tacks"},
                             RESOURCE_DIR "/buttons/upgrades/dart_upgrade2.png",
                             COST_TACK_RANGE_UPGRADE, true)));

  // Ice Ball 升級配置
  s_configs.emplace(
      Tower::TowerType::ice,
      TowerUpgradeConfig(Tower::TowerType::ice,
                         UpgradeButtonConfig(
                             {"Deep", "Freeze"},
                             RESOURCE_DIR "/buttons/upgrades/dart_upgrade1.png",
                             COST_ICE_FREEZE_UPGRADE, true),
                         UpgradeButtonConfig(
                             {"Extra", "Range"},
                             RESOURCE_DIR "/buttons/upgrades/dart_upgrade2.png",
                             COST_ICE_RANGE_UPGRADE, true)));

  // Cannon 升級配置
  s_configs.emplace(
      Tower::TowerType::bomb,
      TowerUpgradeConfig(Tower::TowerType::bomb,
                         UpgradeButtonConfig(
                             {"Bigger", "Bombs"},
                             RESOURCE_DIR "/buttons/upgrades/dart_upgrade1.png",
                             COST_BOMB_SIZE_UPGRADE, true),
                         UpgradeButtonConfig(
                             {"Extra", "Range"},
                             RESOURCE_DIR "/buttons/upgrades/dart_upgrade2.png",
                             COST_BOMB_RANGE_UPGRADE, true)));

  // Super Monkey 升級配置
  s_configs.emplace(
      Tower::TowerType::super,
      TowerUpgradeConfig(Tower::TowerType::super,
                         UpgradeButtonConfig(
                             {"Epic", "Range"},
                             RESOURCE_DIR "/buttons/upgrades/dart_upgrade1.png",
                             COST_SUPER_RANGE_UPGRADE, true),
                         UpgradeButtonConfig(
                             {"Laser", "Vision"},
                             RESOURCE_DIR "/buttons/upgrades/dart_upgrade2.png",
                             COST_SUPER_LASER_UPGRADE, true)));

  LOG_INFO("UPGRADE_BTN_CONF: 已初始化 {} 個塔升級配置", s_configs.size());
}

const TowerUpgradeConfig &
UpgradeButtonConfigManager::GetConfig(Tower::TowerType type) {
  auto it = s_configs.find(type);
  if (it != s_configs.end()) {
    return it->second;
  }

  LOG_WARN("UPGRADE_BTN_CONF: 找不到塔類型 {} 的升級配置",
           static_cast<int>(type));
  // 返回第一個可用的配置作為預設值
  return s_configs.begin()->second;
}

const UpgradeButtonConfig &
UpgradeButtonConfigManager::GetUpgrade1Config(Tower::TowerType type) {
  return GetConfig(type).upgradeButtons[0];
}

const UpgradeButtonConfig &
UpgradeButtonConfigManager::GetUpgrade2Config(Tower::TowerType type) {
  return GetConfig(type).upgradeButtons[1];
}

void UpgradeButtonConfigManager::SetUpgradeAvailability(Tower::TowerType type,
                                                        int upgradeIndex,
                                                        bool available) {
  auto it = s_configs.find(type);
  if (it != s_configs.end() && upgradeIndex >= 0 && upgradeIndex < 2) {
    it->second.upgradeButtons[upgradeIndex].available = available;
    LOG_DEBUG("UPGRADE_BTN_CONF: 設置塔類型 {} 升級 {} 可用性為 {}",
              static_cast<int>(type), upgradeIndex, available);
  } else {
    LOG_WARN("UPGRADE_BTN_CONF: 無法設置升級可用性 - 塔類型: {}, 升級索引: {}",
             static_cast<int>(type), upgradeIndex);
  }
}

void UpgradeButtonConfigManager::SetAllUpgradesAvailability(
    Tower::TowerType type, bool available) {
  auto it = s_configs.find(type);
  if (it != s_configs.end()) {
    it->second.upgradeButtons[0].available = available;
    it->second.upgradeButtons[1].available = available;
    LOG_DEBUG("UPGRADE_BTN_CONF: 設置塔類型 {} 所有升級可用性為 {}",
              static_cast<int>(type), available);
  } else {
    LOG_WARN("UPGRADE_BTN_CONF: 找不到塔類型 {} 無法設置升級可用性",
             static_cast<int>(type));
  }
}

bool UpgradeButtonConfigManager::IsUpgradeAvailable(Tower::TowerType type,
                                                    int upgradeIndex) {
  auto it = s_configs.find(type);
  if (it != s_configs.end() && upgradeIndex >= 0 && upgradeIndex < 2) {
    return it->second.upgradeButtons[upgradeIndex].available;
  }

  LOG_WARN("UPGRADE_BTN_CONF: 檢查升級可用性失敗 - 塔類型: {}, 升級索引: {}",
           static_cast<int>(type), upgradeIndex);
  return false;
}

} // namespace UI
