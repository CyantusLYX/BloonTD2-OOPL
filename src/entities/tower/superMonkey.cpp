#include "entities/tower/superMonkey.hpp"
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "components/canBuy.hpp"
#include "entities/tower/tower.hpp"

SuperMonkey::SuperMonkey(const Util::PTSDPosition &position, float range)
    : Tower::Tower(),
      m_collision(Components::CollisionComponent(position, range)) {

  // 明確設置狀態為 ready（預設塔可以攻擊）
  m_state = ::Tower::TowerState::ready;

  // 設置塔類型
  m_type = ::Tower::TowerType::super;

  // 創建塔身
  m_body = std::make_shared<::Tower::Body>(
      position,
      std::make_shared<Util::Image>(RESOURCE_DIR "/towers/super_monkey.png"));

  // 創建攻擊範圍指示器
  m_range = std::make_shared<::Tower::Range>(range, position);
  // 默認隱藏範圍
  m_range->setVisible(false);

  // 初始化塔資訊
  m_info = {
      "Super Monkey",                // 塔的名稱
      ::Tower::AtkSpeed::HyperSonic, // 攻擊速度 - 最快
      range,                         // 攻擊範圍
      false,                         // 是否有第一個升級 (Range)
      false,                         // 是否有第二個升級 (Laser)
      COST_SUPER                     // 投資成本
  };
}

void SuperMonkey::handleBloonsInRange(
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
  float dartSpeed = 15.0f; // 飛鏢速度 - 比 DartMonkey 更快
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
  const auto &paths = getPaths();
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
  auto dart = std::make_shared<Dart>(
      getPosition(),                 // 從猴子位置發射
      futurePosition,                // 朝向預測的未來位置
      m_info.secondUpgrade ? 2 : 1); // Laser upgrade 增加穿透力

  // 應用第二升級效果 (Laser) - 跟 DartMonkey 的 piercing dart 一樣
  if (m_info.secondUpgrade) {
    dart->setCanPopFrozen(true); // Laser 可以融化冰凍氣球
  }

  // 使用 popperCallback 將飛鏢加入 popper 列表
  if (m_popperCallback) {
    m_popperCallback(dart);
    m_state = ::Tower::TowerState::ready;
  }
}
