#include "entities/tower/dartMonkey.hpp"
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "components/canBuy.hpp"
#include "conf.hpp"
#include "entities/tower/tower.hpp"

DartMonkey::DartMonkey(const Util::PTSDPosition &position, float range)
    : Tower::Tower(),
      m_collision(Components::CollisionComponent(position, range)) {

  // 明確設置狀態為 ready（預設塔可以攻擊）
  m_state = ::Tower::TowerState::ready;

  // 創建塔身
  m_body = std::make_shared<::Tower::Body>(
      position,
      std::make_shared<Util::Image>(RESOURCE_DIR "/towers/dart_monkey.png"));

  // 創建攻擊範圍指示器
  m_range = std::make_shared<::Tower::Range>(range, position);
  // 默認隱藏範圍
  m_range->setVisible(false);
  m_info = {
      "Dart Monkey",             // 塔的名稱
      ::Tower::AtkSpeed::Fast, // 攻擊速度
      range,                     // 攻擊範圍
      false,                     // 是否有第一個升級
      false,                     // 是否有第二個升級
      COST_DART                  // 投資成本
  };
}

void DartMonkey::handleBloonsInRange(
    const std::vector<std::shared_ptr<Bloon>> &bloons,
    const std::vector<float> &distances) {

  // 如果沒有氣球或處於冷卻中，則不執行
  if (bloons.empty() || m_state != ::Tower::TowerState::ready) {
    return;
  }

  // 檢查冷卻時間
  if (m_currentCooldown > 0) {
    m_currentCooldown--;
    return;
  }

  // 找出最遠的氣球作為目標
  float maxDistance = -1.0f;
  size_t targetIndex = 0;

  for (size_t i = 0; i < bloons.size(); ++i) {
    if (distances[i] > maxDistance) {
      maxDistance = distances[i];
      targetIndex = i;
    }
  }

  // 取得目標氣球
  auto targetBloon = bloons[targetIndex];
  
  float bloonDistance = distances[targetIndex];

  // 計算預測幀數 - 根據距離計算飛鏢飛行時間
  float dartSpeed = 10.0f; // 飛鏢速度
  float distance = glm::distance(getPosition().ToVec2(),
                                 targetBloon->getPosition().ToVec2());

  // 計算飛鏢到達目標需要的幀數
  float timeToTarget = distance / dartSpeed;

  // 根據氣球移動速度和預計飛行時間計算未來位置
  float futureDistance =
      bloonDistance + targetBloon->GetSpeed() * timeToTarget - 2.0f;

  // 使用路徑計算未來位置
  Util::PTSDPosition futurePosition = targetBloon->getPosition();
  
  // 從paths vector中根據bloon的path_id找到對應路徑
  int bloonPathId = targetBloon->getPathId();
  const auto& paths = getPaths();
  if (bloonPathId < static_cast<int>(paths.size()) && paths[bloonPathId]) {
    futurePosition = paths[bloonPathId]->getPositionAtDistance(futureDistance);
  }

  // 設置冷卻
  m_currentCooldown = m_cooldown;
  m_state = ::Tower::TowerState::cooldown;
  float deltaX = futurePosition.x - getPosition().x;
  float deltaY = futurePosition.y - getPosition().y;
  m_body->m_Transform.rotation = atan2(deltaY, deltaX);
  m_body->m_Transform.rotation -= M_PI / 2; // 調整旋轉角度
  // 建立飛鏢 - 瞄準預測位置而非當前位置
  auto dart = std::make_shared<Dart>(getPosition(),  // 從猴子位置發射
                                     futurePosition, // 朝向預測的未來位置
                                     m_info.secondUpgrade ? 2 : 1);
  if (m_info.firstUpgrade) {
    dart->setCanPopFrozen(true);
  }
  // 使用 popperCallback 將飛鏢加入 popper 列表
  if (m_popperCallback) {
    m_popperCallback(dart);
    m_state = ::Tower::TowerState::ready;
  }

  // dragging
}