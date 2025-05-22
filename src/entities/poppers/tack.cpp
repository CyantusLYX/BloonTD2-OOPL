#include "entities/poppers/tack.hpp"
#include "Util/Image.hpp"
#include "Util/Logger.hpp"
#include "Util/Position.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <cmath>

Tack::Tack(const Util::PTSDPosition &start, float angle, float maxDistance)
    : popper(start, 15.0f), // 使用較小的碰撞半徑
      m_object(std::make_shared<Util::GameObject>(
          std::make_shared<Util::Image>(RESOURCE_DIR "/poppers/tack.png"), 5.0f)),
      m_maxDistance(maxDistance),
      m_movedDistance(0.0f) {
    
    // 設置物件初始位置和可見性
    m_object->SetVisible(true);
    m_object->m_Transform.translation = glm::vec2(start.x, start.y);
    m_position = start;
    
    // 創建圓形碰撞組件
    m_collisionComponent = std::make_shared<Components::CollisionComponent>(
        start, 8.0f); 
    
    // 根據角度計算方向向量
    m_direction = glm::vec2(std::cos(angle), std::sin(angle));
    
    // 根據方向設置物件旋轉
    m_object->m_Transform.rotation = angle;
    m_object->m_Transform.rotation -= M_PI / 2; // 調整旋轉角度
}

void Tack::setPosition(const Util::PTSDPosition &position) {
    // 更新所有相關位置
    m_position = position;
    
    if (m_object) {
        m_object->m_Transform.translation = glm::vec2(position.x, position.y);
    }
    
    if (m_collisionComponent) {
        m_collisionComponent->setPosition(position);
    }
}

void Tack::move() {
    // 保存原始位置用於計算移動距離
    Util::PTSDPosition old_pos = m_position;
    
    // 使用 next_position 計算下一個位置
    Util::PTSDPosition new_pos = next_position();
    
    // 更新位置
    setPosition(new_pos);
    
    // 計算這一步移動的距離
    float step_distance = glm::distance(
        glm::vec2(old_pos.x, old_pos.y), 
        glm::vec2(new_pos.x, new_pos.y)
    );
    m_movedDistance += step_distance;
    
    // 檢查是否超出最大移動距離
    if (m_movedDistance >= m_maxDistance) {
        this->kill(); // 達到最大距離，摧毀 tack
        return;
    }
}

Util::PTSDPosition Tack::get_position() const {
    return m_position;
}

Util::PTSDPosition Tack::next_position(int frames) {
    // 計算下一個位置
    float next_x = m_position.x + m_direction.x * speed * static_cast<float>(frames);
    float next_y = m_position.y + m_direction.y * speed * static_cast<float>(frames);
    
    return Util::PTSDPosition(next_x, next_y);
}

std::vector<bool> Tack::hit(std::vector<std::shared_ptr<Bloon>> bloons) {
    std::vector<bool> hit_results(bloons.size(), false);
    
    for (size_t i = 0; i < bloons.size(); i++) {
        // Tack 只有一滴生命值，打到一個氣球就消失
        hit_results[i] = true; // 標記此氣球會被擊破
        this->kill();          // 直接標記為死亡
        break;                 // 不需要檢查其他氣球
    }
    
    return hit_results;
}

std::shared_ptr<Util::GameObject> Tack::get_object() {
    return m_object;
}