#ifndef TOWER_BTN_CONF_HPP
#define TOWER_BTN_CONF_HPP

#include "components/towerType.hpp"
#include <string>
#include <vector>
#include <unordered_map>

namespace UI {

// 塔按鈕配置結構
struct TowerButtonConfig {
    Tower::TowerType type;      // 塔類型
    std::string name;           // 按鈕名稱
    std::string imagePath;      // 圖片路徑
    int cost;                   // 花費
    bool available;             // 是否可用
    
    // 構造函數
    TowerButtonConfig(Tower::TowerType _type, 
                     const std::string& _name, 
                     const std::string& _imagePath, 
                     int _cost, 
                     bool _available = true)
        : type(_type), name(_name), imagePath(_imagePath), 
          cost(_cost), available(_available) {}
};

// 塔按鈕配置管理類
class TowerButtonConfigManager {
private:
    static std::unordered_map<Tower::TowerType, TowerButtonConfig> s_configs;

public:
    // 初始化所有配置
    static void Initialize();
    
    // 獲取指定類型的配置
    static const TowerButtonConfig& GetConfig(Tower::TowerType type);
    
    // 獲取所有可用的配置
    static std::vector<TowerButtonConfig> GetAllAvailableConfigs();
    
    // 設置某類型的可用性
    static void SetAvailability(Tower::TowerType type, bool available);
};

} // namespace UI

#endif // TOWER_BTN_CONF_HPP