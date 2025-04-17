#ifndef DARTMONKEY_HPP
#define DARTMONKEY_HPP
#include "Util/Time.hpp"
#include "entities/poppers/dart.hpp"
#include "tower.hpp"

class DartMonkey : public Tower::Tower {
private:
  int m_cooldown = 25;       // 射擊冷卻時間（幀數）
  int m_currentCooldown = 0; // 當前剩餘冷卻時間

public:
  DartMonkey(const Util::PTSDPosition &position, float range = 150.0f);
  ~DartMonkey() = default;
  
  // 處理射程內氣球的方法
  void handleBloonsInRange(const std::vector<std::shared_ptr<Bloon>> &bloons,
                           const std::vector<float> &distances) override;
};
#endif