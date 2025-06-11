#include "entities/tower/ice_ball.hpp"
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "components/canBuy.hpp"
#include "entities/tower/tower.hpp"

IceBall::IceBall(const Util::PTSDPosition &position, float range)
    : Tower::Tower(),
      m_collision(Components::CollisionComponent(position, range)) {

  // 設置塔類型
  m_type = ::Tower::TowerType::ice;

  // 建立塔身
  m_body = std::make_shared<::Tower::Body>(
      position,
      std::make_shared<Util::Image>(RESOURCE_DIR "/towers/ice_ball.png"));

  // 建立範圍顯示
  m_range = std::make_shared<::Tower::Range>(range, position);
  // 默認隱藏範圍
  m_range->setVisible(false);

  // 設置碰撞範圍
  m_collision.setPosition(position);

  // 設置初始狀態
  m_state = ::Tower::TowerState::ready;

  // 初始化塔資訊
  m_info = {
      "Ice Ball",              // 塔的名稱
      ::Tower::AtkSpeed::Slow, // 攻擊速度
      range,                   // 攻擊範圍
      false,                   // 是否有第一個升級
      false,                   // 是否有第二個升級
      COST_ICE                 // 投資成本
  };

  // 設置路徑
  setPath(nullptr); // IceBall 不需要路徑用於預測
}

void IceBall::handleBloonsInRange(
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

  // 如果沒有氣球，直接返回（注意：已經移除對狀態的檢查）
  if (bloons.empty()) {
    return;
  }

  // 凍結所有範圍內的氣球
  bool anyFrozen = false;
  for (size_t i = 0; i < bloons.size(); ++i) {
    auto targetBloon = bloons[i];

    // 檢查氣球類型
    Bloon::Type bloonType = targetBloon->GetType();

    // 預設可凍結大多數氣球，但特定類型需要升級才能凍結
    bool canFreeze = true;

    // 白色氣球免疫冰凍
    if (bloonType == Bloon::Type::white && !m_canFreezeWhite) {
      canFreeze = false;
    }

    // 鉛氣球免疫冰凍
    if (bloonType == Bloon::Type::lead && !m_canFreezeLead) {
      canFreeze = false;
    }

    // 如果氣球已經凍結，不再重複凍結
    if (targetBloon->GetState() == Bloon::State::frozed) {
      canFreeze = false;
    }

    // 凍結可凍結的氣球
    if (canFreeze) {
      targetBloon->setFrozed(m_freezeFrames);
      anyFrozen = true;
    }
  }

  // 如果有任何氣球被凍結，進入冷卻狀態
  if (anyFrozen) {
    // 設置冷卻
    m_currentCooldown = m_cooldown;
    m_state = ::Tower::TowerState::cooldown;

    // 可以添加視覺效果，例如閃爍或動畫
    LOG_DEBUG("ICBAL : Froze {} bloons for {} frames", bloons.size(),
              m_freezeFrames);
  }
}
