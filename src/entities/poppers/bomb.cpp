#include "entities/poppers/bomb.hpp"
#include "Util/Image.hpp"
#include "Util/Position.hpp"
#include "Util/Logger.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <cmath>

Bomb::Bomb(const Util::PTSDPosition &start, float angle, 
           float triggerRadius, float explosionRadius, float maxDistance)
    : popper(start, explosionRadius), // 使用爆炸半徑作為基礎碰撞組件
      m_object(std::make_shared<Util::GameObject>(
          std::make_shared<Util::Image>(RESOURCE_DIR "/poppers/bomb.png"), 5.0f)),
      m_maxDistance(maxDistance) {
    
    // 設置物件初始位置和可見性
    m_object->SetVisible(true);
    m_object->m_Transform.translation = glm::vec2(start.x, start.y);
    m_position = start;
    
    // 創建觸發碰撞組件（較小半徑）
    m_triggerComponent = std::make_shared<Components::CollisionComponent>(
        start, triggerRadius);
    
    // 創建爆炸碰撞組件（較大半徑）
    m_explosionComponent = std::make_shared<Components::CollisionComponent>(
        start, explosionRadius);
    
    // 根據角度計算方向向量
    m_direction = glm::vec2(std::cos(angle), std::sin(angle));
    
    // 根據方向設置物件旋轉
    m_object->m_Transform.rotation = angle;
    m_object->m_Transform.rotation -= M_PI / 2; // 調整旋轉角度
    
    // 設置為爆炸類型
    explosive = true;
}

void Bomb::setPosition(const Util::PTSDPosition &position) {
    // 更新所有相關位置
    m_position = position;
    
    if (m_object) {
        m_object->m_Transform.translation = glm::vec2(position.x, position.y);
    }
    
    if (m_triggerComponent) {
        m_triggerComponent->setPosition(position);
    }
    
    if (m_explosionComponent) {
        m_explosionComponent->setPosition(position);
    }
}

void Bomb::move() {
    // 如果已爆炸或不再移動，則直接返回
    if (m_exploded || !m_isMoving) {
        return;
    }
    
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
    
    // 檢查是否達到最大距離
    if (m_movedDistance >= m_maxDistance) {
        this->kill();
    }
    
    // 檢查螢幕邊界
    const float BOUND_LIMIT = 800.0f;
    if (std::abs(m_position.x) > BOUND_LIMIT || std::abs(m_position.y) > BOUND_LIMIT) {
        this->kill();
    }
}

Util::PTSDPosition Bomb::get_position() const {
    return m_position;
}

Util::PTSDPosition Bomb::next_position(int frames) {
    // 計算下一個位置
    float next_x = m_position.x + m_direction.x * speed * static_cast<float>(frames);
    float next_y = m_position.y + m_direction.y * speed * static_cast<float>(frames);
    
    return Util::PTSDPosition(next_x, next_y);
}

bool Bomb::isInTriggerRange(const std::shared_ptr<Bloon>& bloon) const {
    // 檢查氣球是否在觸發範圍內
    if (m_triggerComponent && bloon) {
        return m_triggerComponent->isCollide(*bloon);
    }
    return false;
}

void Bomb::explode() {
    if (m_exploded) {
        return; // 已經爆炸過
    }
    
    m_exploded = true;
    m_isMoving = false;
    
    LOG_DEBUG("BOMB  : 炸彈爆炸在 ({}, {})", m_position.x, m_position.y);
}

std::vector<bool> Bomb::hit(std::vector<std::shared_ptr<Bloon>> bloons) {
    std::vector<bool> hit_results(bloons.size(), false);
    
    // 檢查是否有氣球在觸發範圍內
    bool shouldExplode = false;
    for (size_t i = 0; i < bloons.size(); i++) {
        if (isInTriggerRange(bloons[i])) {
            shouldExplode = true;
            break;
        }
    }
    
    // 如果觸發了爆炸
    if (shouldExplode) {
        explode();
        
        // 標記所有碰撞的氣球為已擊中
        for (size_t i = 0; i < hit_results.size(); i++) {
            hit_results[i] = true;
        }
    }
    
    // 若已爆炸，確認所有在爆炸範圍內的氣球
    if (m_exploded) {
        
        // 爆炸效果處理完後，標記為死亡
        this->kill();
        return hit_results;
    }
    
    return hit_results;
}

std::shared_ptr<Util::GameObject> Bomb::get_object() {
    return m_object;
}