#ifndef TOWER_CONFIG_HPP
#define TOWER_CONFIG_HPP

#include "entities/tower/tower.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

// 物品類型枚舉
enum class BuyableType {
    Tower,  // 普通塔
    Popper  // 釘子、膠水等
};


// 基本配置結構 - 所有可購買物品共有的屬性
struct BaseConfig {
    std::string name;      // 物品名稱
    int cost;              // 建造成本
    std::string imageFile; // 圖像檔案
    BuyableType type;      // 物品類型
};

// 塔專用配置結構
struct TowerConfig : public BaseConfig {
    float range;           // 攻擊範圍
    std::shared_ptr<SpecConfig> specConfig; // 特定塔的配置
};

// Popper 專用配置結構
struct PopperConfig : public BaseConfig {
    int durability;        // 耐久度/生命值
};
// 配置管理器
class BuyableConfigManager {
private:
    static std::unordered_map<Tower::TowerType, BaseConfig> s_baseConfigs;
    static std::unordered_map<Tower::TowerType, TowerConfig> s_towerConfigs;
    static std::unordered_map<Tower::TowerType, PopperConfig> s_popperConfigs;
    static bool s_initialized;

public:
    // 初始化所有物品配置
    static void Initialize();
    
    // 獲取物品類型
    static BuyableType GetItemType(Tower::TowerType type);
    
    // 獲取物品基本配置
    static const BaseConfig& GetBaseConfig(Tower::TowerType type);
    
    // 獲取塔配置
    static const TowerConfig& GetTowerConfig(Tower::TowerType type);
    
    // 獲取 Popper 配置
    static const PopperConfig& GetPopperConfig(Tower::TowerType type);
    
    // 獲取物品成本
    static int GetCost(Tower::TowerType type);
    
    // 檢查是否為塔類型
    static bool IsTower(Tower::TowerType type);
    
    // 檢查是否為 Popper 類型
    static bool IsPopper(Tower::TowerType type);
};

#endif // TOWER_CONFIG_HPP