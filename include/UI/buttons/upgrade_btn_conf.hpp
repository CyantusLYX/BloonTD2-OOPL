#ifndef UPGRADE_BTN_CONF_HPP
#define UPGRADE_BTN_CONF_HPP

#include "components/towerType.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <array>

namespace UI {

// 單個升級按鈕配置結構
struct UpgradeButtonConfig {
    std::vector<std::string> name;    // 按鈕名稱（支援多行文字）
    std::string imagePath;            // 圖片路徑
    int cost;                         // 升級花費
    bool available;                   // 是否可用
    
    // 構造函數
    UpgradeButtonConfig(const std::vector<std::string>& _name, 
                       const std::string& _imagePath, 
                       int _cost, 
                       bool _available = true)
        : name(_name), imagePath(_imagePath), 
          cost(_cost), available(_available) {}
    
    // 預設構造函數
    UpgradeButtonConfig() : cost(0), available(false) {}
};

// 塔升級按鈕配置結構（包含兩個升級按鈕）
struct TowerUpgradeConfig {
    Tower::TowerType type;                               // 塔類型
    std::array<UpgradeButtonConfig, 2> upgradeButtons;   // 兩個升級按鈕配置
    
    // 構造函數
    TowerUpgradeConfig(Tower::TowerType _type,
                      const UpgradeButtonConfig& _upgrade1,
                      const UpgradeButtonConfig& _upgrade2)
        : type(_type), upgradeButtons{_upgrade1, _upgrade2} {}
    
    // 預設構造函數
    TowerUpgradeConfig() : type(Tower::TowerType::dart) {}
};

// 升級按鈕配置管理類
class UpgradeButtonConfigManager {
private:
    static std::unordered_map<Tower::TowerType, TowerUpgradeConfig> s_configs;

public:
    // 初始化所有配置
    static void Initialize();
    
    // 獲取指定類型的升級配置
    static const TowerUpgradeConfig& GetConfig(Tower::TowerType type);
    
    // 獲取指定類型的第一個升級按鈕配置
    static const UpgradeButtonConfig& GetUpgrade1Config(Tower::TowerType type);
    
    // 獲取指定類型的第二個升級按鈕配置
    static const UpgradeButtonConfig& GetUpgrade2Config(Tower::TowerType type);
    
    // 設置某類型某個升級按鈕的可用性
    static void SetUpgradeAvailability(Tower::TowerType type, int upgradeIndex, bool available);
    
    // 設置某類型所有升級按鈕的可用性
    static void SetAllUpgradesAvailability(Tower::TowerType type, bool available);
    
    // 檢查某個升級是否可用
    static bool IsUpgradeAvailable(Tower::TowerType type, int upgradeIndex);
};

} // namespace UI

#endif // UPGRADE_BTN_CONF_HPP
