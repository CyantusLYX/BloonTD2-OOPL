#include "entities/poppers/boomerang.hpp"
#include "Util/Image.hpp"
#include "Util/Logger.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

Boomerang::Boomerang(const Util::PTSDPosition &start, float angle, float diameter, int life)
    : popper(start, 10.0f),  // 設置碰撞半徑為 10.0f
      m_startPos(start),
      m_angle(angle),
      m_diameter(diameter),
      m_currentAngle(-M_PI),  // 從 0 開始，即圓的起點
      m_angularSpeed(0.21f), // 每幀增加的角度，可調整
      m_life(life),
      m_hasCompletedPath(false) {
    
    // 設置物件初始位置和可見性
    m_object = std::make_shared<Util::GameObject>(
        std::make_shared<Util::Image>(RESOURCE_DIR "/poppers/boomerang.png"), 5.0f);
    m_object->SetVisible(true);
    m_object->m_Transform.translation = glm::vec2(start.x, start.y);
    
    // 初始旋轉設置
    m_object->m_Transform.rotation = angle;
    
    // 創建碰撞組件
    m_collisionComponent = std::make_shared<Components::CollisionComponent>(
        start, 10.0f);
    
    // 初始化位置
    m_position = start;
    
    LOG_DEBUG("BOMRG : 創建迴旋鏢 - 直徑: {}, 生命值: {}", diameter, life);
}

void Boomerang::move() {
    // 增加當前角度
    m_currentAngle += m_angularSpeed;
    
    // 計算圓的中心位置（在塔面對方向上的一個點，而不是塔本身）
    float radius = m_diameter / 2.0f;
    float circleCenterX = m_startPos.x + radius * cos(m_angle);
    float circleCenterY = m_startPos.y + radius * sin(m_angle);
    
    // 計算迴旋鏢在圓上的位置
    float newX = circleCenterX + radius * cos(m_angle + m_currentAngle);
    float newY = circleCenterY + radius * sin(m_angle + m_currentAngle);
    
    // 更新位置
    setPosition(Util::PTSDPosition(newX, newY));
    
    // 檢查是否完成一整個圓
    if (m_currentAngle >= 1 * M_PI) {
        m_hasCompletedPath = true;
        this->kill();  // 完成路徑後消失
        LOG_DEBUG("BOMRG : 迴旋鏢完成一圈路徑，消失");
    }
    
    // 旋轉迴旋鏢自身（視覺效果）
    if (m_object) {
        m_object->m_Transform.rotation += 0.2f;  // 自旋速度
    }
}

Util::PTSDPosition Boomerang::next_position(int frames) {
    // 計算若干幀後的角度
    float futureAngle = m_currentAngle + m_angularSpeed * static_cast<float>(frames);
    
    // 計算圓的中心位置
    float radius = m_diameter / 2.0f;
    float circleCenterX = m_startPos.x + radius * cos(m_angle);
    float circleCenterY = m_startPos.y + radius * sin(m_angle);
    
    // 計算未來位置
    float nextX = circleCenterX + radius * cos(m_angle + futureAngle);
    float nextY = circleCenterY + radius * sin(m_angle + futureAngle);
    
    return Util::PTSDPosition(nextX, nextY);
}

void Boomerang::setPosition(const Util::PTSDPosition &position) {
    // 更新所有相關位置
    m_position = position;
    
    if (m_object) {
        m_object->m_Transform.translation = glm::vec2(position.x, position.y);
    }
    
    if (m_collisionComponent) {
        m_collisionComponent->setPosition(position);
    }
}

Util::PTSDPosition Boomerang::get_position() const {
    return m_position;
}

std::vector<bool> Boomerang::hit(std::vector<std::shared_ptr<Bloon>> bloons) {
    std::vector<bool> hit_results(bloons.size(), false);
    
    // 如果已用完所有生命值或完成路徑，則移除
    if (m_life <= 0 || m_hasCompletedPath) {
        this->kill();
        return hit_results;
    }
    
    // 處理與氣球的碰撞
    for (size_t i = 0; i < bloons.size(); i++) {
        // 標記擊中
        hit_results[i] = true;
        
        // 減少剩餘生命值
        m_life--;
        
        // 如果用完所有生命值，標記為死亡並中斷
        if (m_life <= 0) {
            this->kill();
            LOG_DEBUG("BOMRG : 迴旋鏢耗盡生命值，消失");
            break;
        }
    }
    
    return hit_results;
}

std::shared_ptr<Util::GameObject> Boomerang::get_object() {
    return m_object;
}