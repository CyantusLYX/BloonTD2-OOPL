#include "entities/tower/boomerang_monkey.hpp"
#include "Util/Image.hpp"
#include "Util/Logger.hpp"
#include "entities/poppers/boomerang.hpp"
#include <cmath>

BoomerangMonkey::BoomerangMonkey(const Util::PTSDPosition &position,
                                 float range)
    : Tower::Tower(),
      m_collision(Components::CollisionComponent(position, range)) {

  // 設置狀態為 ready
  m_state = ::Tower::TowerState::ready;

  // 設置類型
  m_type = ::Tower::TowerType::boomerang;

  // 創建塔身
  m_body = std::make_shared<::Tower::Body>(
      position, std::make_shared<Util::Image>(RESOURCE_DIR
                                              "/towers/boomerang_monkey.png"));

  // 創建攻擊範圍指示器
  m_range = std::make_shared<::Tower::Range>(range, position);
  // 默認隱藏範圍
  m_range->setVisible(false);

  // 初始化塔資訊
  m_info = {
      "Boomerang Monkey",      // 塔的名稱
      ::Tower::AtkSpeed::Slow, // 攻擊速度
      range,                   // 攻擊範圍
      false,                   // 是否有第一個升級
      false,                   // 是否有第二個升級
      COST_BOOMERANG           // 投資成本
  };
}

void BoomerangMonkey::handleBloonsInRange(
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

  // 進行預測性瞄準
  auto moveComponent =
      std::dynamic_pointer_cast<Interface::I_move>(targetBloon);
  if (moveComponent) {
    // 預測氣球的位置
    Util::PTSDPosition predictedPos = moveComponent->next_position(10);
    targetPos = predictedPos;
  }

  // 計算朝向目標的角度
  float deltaX = targetPos.x - position.x;
  float deltaY = targetPos.y - position.y;
  float angle = atan2(deltaY, deltaX);

  // 旋轉塔身指向目標
  m_body->m_Transform.rotation = angle - M_PI / 2;

  // 創建迴旋鏢 - 使用塔的范圍作為直徑
  auto boomerang = std::make_shared<Boomerang>(
      position, angle, m_range->getRadius(), m_boomerangLife);

  // 如果有第二個升級 (Sonic Boom)，設置可以擊破冰凍氣球
  if (m_info.secondUpgrade) {
    boomerang->setCanPopFrozen(true);
  }

  // 使用回調函數將迴旋鏢加入遊戲世界
  if (m_popperCallback) {
    m_popperCallback(boomerang);
  }

  // 設置冷卻
  m_currentCooldown = m_cooldown;
  m_state = ::Tower::TowerState::cooldown;

  LOG_DEBUG("BOMKY : 朝 ({}, {}) 發射迴旋鏢", targetPos.x, targetPos.y);
}
