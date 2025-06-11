#include "entities/poppers/dart.hpp"
#include "Util/Image.hpp"
#include "Util/Position.hpp"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

Dart::Dart(const Util::PTSDPosition &start, const Util::PTSDPosition &end,
           int life)
    : popper(start, 10.0f), // 使用起始點作為基礎位置
      m_object(std::make_shared<Util::GameObject>(
          std::make_shared<Util::Image>(RESOURCE_DIR "/poppers/dart.png"),
          5.0f)),
      life(life) {

  // 設置物件初始位置和可見性
  m_object->SetVisible(true);
  m_object->m_Transform.translation = glm::vec2(start.x, start.y);
  m_position = start;

  // 創建圓形碰撞組件（假設飛鏢的碰撞區域是半徑5的圓）
  m_collisionComponent = std::make_shared<Components::CollisionComponent>(
      start, 5.0f); // 使用整數表示圓形半徑

  // 計算方向向量（從起點指向終點的單位向量）
  glm::vec2 direction_vector = glm::vec2(end.x - start.x, end.y - start.y);

  // 如果方向向量長度不為零，則正規化
  if (glm::length2(direction_vector) > 0.00001f) {
    m_direction = glm::normalize(direction_vector);
  } else {
    // 預設方向（右方）
    m_direction = glm::vec2(1.0f, 0.0f);
  }

  // 根據方向設置物件旋轉
  float rotation_angle = atan2(m_direction.y, m_direction.x);
  m_object->m_Transform.rotation = rotation_angle;
  m_object->m_Transform.rotation -= M_PI / 2;
}

// 實現 setPosition 方法 - 同時更新多個位置相關的元素
void Dart::setPosition(const Util::PTSDPosition &position) {
  // 1. 更新 popper 基類中的位置
  m_position = position;

  // 2. 更新 GameObject 的位置
  if (m_object) {
    m_object->m_Transform.translation = glm::vec2(position.x, position.y);
  }

  // 3. 更新碰撞組件的位置
  if (m_collisionComponent) {
    m_collisionComponent->setPosition(position);
  }
}

// 更新 move 方法使用 setPosition
void Dart::move() {
  // 使用 next_position 計算下一個位置
  Util::PTSDPosition new_pos = next_position();

  // 使用新的 setPosition 方法更新位置
  setPosition(new_pos);

  // 檢查邊界 (例如: -800 到 800 的螢幕範圍)
  const float BOUND_LIMIT = 800.0f;
  if (std::abs(m_position.x) > BOUND_LIMIT ||
      std::abs(m_position.y) > BOUND_LIMIT) {
    this->kill(); // 超出邊界則摧毀飛鏢
    is_outside_bounds = true;
  }
}

// 實現其他必要的方法

Util::PTSDPosition Dart::get_position() const {
  return m_position;
}

Util::PTSDPosition Dart::next_position(int frames) {
  // 計算下一個位置（當前位置 + 方向*速度*幀數）
  float next_x =
      m_position.x + m_direction.x * speed * static_cast<float>(frames);
  float next_y =
      m_position.y + m_direction.y * speed * static_cast<float>(frames);

  return Util::PTSDPosition(next_x, next_y);
}

std::vector<bool> Dart::hit(std::vector<std::shared_ptr<Bloon>> bloons) {
  std::vector<bool> hit_results(bloons.size(), false);

  for (size_t i = 0; i < bloons.size(); i++) {
    // 如果飛鏢還有生命值，可以擊破氣球
    if (this->life > 0) {
      hit_results[i] = true; // 標記此氣球會被擊破
      this->life--;          // 減少飛鏢的生命值

      // 如果飛鏢生命值歸零，標記為死亡
      if (this->life <= 0) {
        this->kill();
        break; // 生命值用完，無法再擊破更多氣球
      }
    } else {
      // 生命值已用完，無法擊破更多氣球
      hit_results[i] = false;
    }
  }

  return hit_results;
}

std::shared_ptr<Util::GameObject> Dart::get_object() {
  return m_object;
}
