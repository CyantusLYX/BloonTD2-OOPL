#include "entities/tower/tower_config.hpp"
#include "Util/Logger.hpp"

// 初始化靜態成員
std::unordered_map<Tower::TowerType, BaseConfig> BuyableConfigManager::s_baseConfigs;
std::unordered_map<Tower::TowerType, TowerConfig> BuyableConfigManager::s_towerConfigs;
std::unordered_map<Tower::TowerType, PopperConfig> BuyableConfigManager::s_popperConfigs;
bool BuyableConfigManager::s_initialized = false;

// 初始化所有物品配置
void BuyableConfigManager::Initialize() {
    if (s_initialized) return;
    
    // 添加 DartMonkey 配置
    TowerConfig dartConfig;
    dartConfig.name = "Dart Monkey";
    dartConfig.cost = 250;
    dartConfig.imageFile = RESOURCE_DIR "/towers/dart_monkey.png";
    dartConfig.type = BuyableType::Tower;
    dartConfig.range = 150.0f;
    s_towerConfigs[Tower::TowerType::dart] = dartConfig;
    s_baseConfigs[Tower::TowerType::dart] = dartConfig;
    
    // 添加 BoomerangMonkey 配置
    TowerConfig boomerangConfig;
    boomerangConfig.name = "Boomerang Monkey";
    boomerangConfig.cost = 300;
    boomerangConfig.imageFile = RESOURCE_DIR "/towers/boomerang_monkey.png";
    boomerangConfig.type = BuyableType::Tower;
    boomerangConfig.range = 180.0f;
    s_towerConfigs[Tower::TowerType::boomerang] = boomerangConfig;
    s_baseConfigs[Tower::TowerType::boomerang] = boomerangConfig;
    
    // 添加 Spike 配置
    PopperConfig spikeConfig;
    spikeConfig.name = "Spike";
    spikeConfig.cost = 25;
    spikeConfig.imageFile = RESOURCE_DIR "/poppers/spike.png";
    spikeConfig.type = BuyableType::Popper;
    spikeConfig.durability = 10; // 可以刺破10個氣球
    s_popperConfigs[Tower::TowerType::spike] = spikeConfig;
    s_baseConfigs[Tower::TowerType::spike] = spikeConfig;
    
    // 添加 Glue 配置
    PopperConfig glueConfig;
    glueConfig.name = "Glue";
    glueConfig.cost = 35;
    glueConfig.imageFile = RESOURCE_DIR "/poppers/glue.png";
    glueConfig.type = BuyableType::Popper;
    glueConfig.durability = 5; // 可以減速5個氣球
    s_popperConfigs[Tower::TowerType::glue] = glueConfig;
    s_baseConfigs[Tower::TowerType::glue] = glueConfig;
    
    s_initialized = true;
    LOG_DEBUG("CONFIG : 物品配置已初始化");
}

// 獲取物品類型
BuyableType BuyableConfigManager::GetItemType(Tower::TowerType type) {
    if (!s_initialized) Initialize();
    
    auto it = s_baseConfigs.find(type);
    if (it == s_baseConfigs.end()) {
        LOG_ERROR("CONFIG : 未找到類型 {} 的配置", static_cast<int>(type));
        return BuyableType::Tower; // 默認為塔
    }
    
    return it->second.type;
}

// 獲取物品基本配置
const BaseConfig& BuyableConfigManager::GetBaseConfig(Tower::TowerType type) {
    if (!s_initialized) Initialize();
    
    auto it = s_baseConfigs.find(type);
    if (it == s_baseConfigs.end()) {
        LOG_ERROR("CONFIG : 未找到類型 {} 的基本配置", static_cast<int>(type));
        static BaseConfig defaultConfig = {"Unknown", 0, "", BuyableType::Tower};
        return defaultConfig;
    }
    
    return it->second;
}

// 獲取塔配置
const TowerConfig& BuyableConfigManager::GetTowerConfig(Tower::TowerType type) {
    if (!s_initialized) Initialize();
    
    auto it = s_towerConfigs.find(type);
    if (it == s_towerConfigs.end()) {
        LOG_ERROR("CONFIG : 未找到類型 {} 的塔配置", static_cast<int>(type));
        static TowerConfig defaultConfig = {"Unknown", 0, "", BuyableType::Tower, 0.0f};
        return defaultConfig;
    }
    
    return it->second;
}

// 獲取 Popper 配置
const PopperConfig& BuyableConfigManager::GetPopperConfig(Tower::TowerType type) {
    if (!s_initialized) Initialize();
    
    auto it = s_popperConfigs.find(type);
    if (it == s_popperConfigs.end()) {
        LOG_ERROR("CONFIG : 未找到類型 {} 的 Popper 配置", static_cast<int>(type));
        static PopperConfig defaultConfig = {"Unknown", 0, "", BuyableType::Popper, 0};
        return defaultConfig;
    }
    
    return it->second;
}

// 獲取物品成本
int BuyableConfigManager::GetCost(Tower::TowerType type) {
    return GetBaseConfig(type).cost;
}

bool BuyableConfigManager::IsTower(Tower::TowerType type) {
    // 明確列出所有塔類型
    static const std::unordered_set<Tower::TowerType> towerTypes = {
        Tower::TowerType::dart,
        Tower::TowerType::tack,
        Tower::TowerType::ice,
        Tower::TowerType::bomb,
        Tower::TowerType::boomerang,
        Tower::TowerType::super
        // 其他塔類型...
    };
    return towerTypes.find(type) != towerTypes.end();
}

bool BuyableConfigManager::IsPopper(Tower::TowerType type) {
    // 明確列出所有 popper 類型
    static const std::unordered_set<Tower::TowerType> popperTypes = {
        Tower::TowerType::spike,
        Tower::TowerType::glue
    };
    
    return popperTypes.find(type) != popperTypes.end();
}