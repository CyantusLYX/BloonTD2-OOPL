#include "entities/tower/cannon.hpp"
#include "entities/poppers/bomb.hpp"
#include "Util/Image.hpp"
#include "Util/Logger.hpp"
#include <cmath>

Cannon::Cannon(const Util::PTSDPosition &position, float range)
    : Tower::Tower(),
      m_triggerRadius(20.0f),   // 默認觸發半徑
      m_explosionRadius(60.0f), // 默認爆炸半徑
      m_collision(Components::CollisionComponent(position, range))
{
  // 設置狀態為 ready
  m_state = ::Tower::TowerState::ready;

  // 設置塔類型
  m_type = ::Tower::TowerType::bomb;

  // 創建塔身
  m_body = std::make_shared<::Tower::Body>(
      position,
      std::make_shared<Util::Image>(RESOURCE_DIR "/towers/cannon.png"));

  // 創建攻擊範圍指示器
  m_range = std::make_shared<::Tower::Range>(range, position);
  // 默認隱藏範圍
  m_range->setVisible(false);
  
  // 初始化塔資訊
  m_info = {
      "Cannon",                   // 塔的名稱
      ::Tower::AtkSpeed::Slow,    // 攻擊速度
      range,                      // 攻擊範圍
      false,                      // 是否有第一個升級
      false,                      // 是否有第二個升級
      COST_BOMB                   // 投資成本
  };
}

void Cannon::handleBloonsInRange(
    const std::vector<std::shared_ptr<Bloon>> &bloons,
    const std::vector<float> &distances) {
    
  // 處理冷卻時間
  if (m_currentCooldown > 0) {
      m_currentCooldown--;
      
      // 當冷卻結束時，恢復到 ready 狀態
      if (m_currentCooldown <= 0) {
          m_state = ::Tower::TowerState::ready;
      }
  }
  
  // 如果沒有氣球或塔不在 ready 狀態，直接返回
  if (bloons.empty() || m_state != ::Tower::TowerState::ready) {
      return;
  }

  // 找出最遠的氣球作為目標（領先的氣球）
  size_t targetIndex = 0;
  float maxDistance = distances[0];

  for (size_t i = 1; i < bloons.size(); ++i) {
    if (distances[i] > maxDistance) {
      maxDistance = distances[i];
      targetIndex = i;
    }
  }

  // 獲取目標氣球
  auto targetBloon = bloons[targetIndex];
  
  // 取得氣球當前位置
  Util::PTSDPosition targetPos = targetBloon->getPosition();
  Util::PTSDPosition position = getPosition();
  
  // 進行預測性瞄準 - 考慮氣球移動方向和速度
  // 1. 獲取氣球移動方向和速度
  auto moveComponent = std::dynamic_pointer_cast<Interface::I_move>(targetBloon);
  if (moveComponent) {
    // 計算氣球下一幀的預測位置 (依據炸彈飛行時間來預測)
    float bombTravelTime = glm::distance(glm::vec2(position.x, position.y), 
                                        glm::vec2(targetPos.x, targetPos.y)) / 15.0f; // 炸彈速度為5
    
    Util::PTSDPosition predictedPos = moveComponent->next_position(
        static_cast<int>(bombTravelTime));
    
    // 使用預測位置來瞄準
    targetPos = predictedPos;
  }
  
  // 計算朝向預測位置的方向
  float deltaX = targetPos.x - position.x;
  float deltaY = targetPos.y - position.y;
  float angle = atan2(deltaY, deltaX);
  
  // 旋轉塔身指向目標
  m_body->m_Transform.rotation = angle - M_PI / 2;
  
  // 創建炸彈 - 使用設定的觸發半徑和爆炸半徑 
  auto bomb = std::make_shared<Bomb>(position, angle, m_triggerRadius, m_explosionRadius);
  
  // 使用回調函數將炸彈加入遊戲世界
  if (m_popperCallback) {
      m_popperCallback(bomb);
  }
  
  // 設置冷卻
  m_currentCooldown = m_cooldown;
  m_state = ::Tower::TowerState::cooldown;
  
  LOG_DEBUG("CANNON: 朝 ({}, {}) 發射炸彈, 目標氣球離起點距離: {}", 
            targetPos.x, targetPos.y, maxDistance);
}