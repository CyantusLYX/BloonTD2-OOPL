#include "entities/tower/tack_shooter.hpp"
#include "conf.hpp"
#include "entities/poppers/tack.hpp"
#include "Util/Image.hpp"
#include "Util/Logger.hpp"
#include <cmath>

TackShooter::TackShooter(const Util::PTSDPosition &position, float range)
    : Tower::Tower(),
      m_collision(Components::CollisionComponent(position, range)) {
  
  // 明確設置狀態為 ready（預設塔可以攻擊）
  m_state = ::Tower::TowerState::ready;

  // 創建塔身
  m_body = std::make_shared<::Tower::Body>(
      position,
      std::make_shared<Util::Image>(RESOURCE_DIR "/towers/tack_shooter.png"));

  // 創建攻擊範圍指示器
  m_range = std::make_shared<::Tower::Range>(range, position);
  // 默認隱藏範圍
  m_range->setVisible(false);
}

void TackShooter::handleBloonsInRange(
    const std::vector<std::shared_ptr<Bloon>> &bloons,
    const std::vector<float> &distances) {
    
  // 處理冷卻時間，無論當前狀態如何
  if (m_currentCooldown > 0) {
      m_currentCooldown--;
      
      // 當冷卻結束時，恢復到 ready 狀態
      if (m_currentCooldown <= 0) {
          m_state = ::Tower::TowerState::ready;
      }
      
      // 如果還在冷卻中，直接返回
      if (m_state != ::Tower::TowerState::ready) {
          return;
      }
  }
  
  // 如果沒有氣球，直接返回
  if (bloons.empty()) {
      return;
  }

  // 獲取發射位置
  auto position = getPosition();
  
  // 發射 8 個方向的 tack
  for (int i = 0; i < 8; i++) {
    // 計算每個 tack 的角度 (45度間隔)
    float angle = i * 45.0f * (M_PI / 180.0f);
    
    // 建立 tack
    auto tack = std::make_shared<Tack>(position, angle, m_range->getRadius()/2);
    
    // 使用回調函數將 tack 加入遊戲世界
    if (m_popperCallback) {
      m_popperCallback(tack);
    }
  }
  
  // 設置冷卻
  m_currentCooldown = m_cooldown;
  m_state = ::Tower::TowerState::cooldown;
}