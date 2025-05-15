#ifndef CANNON_HPP
#define CANNON_HPP

#include "conf.hpp"
#include "entities/tower/tower.hpp"
#include "entities/bloon.hpp"
#include "components/collisionComp.hpp"
#include <memory>
#include "conf.hpp"
#include "config.hpp"

class Cannon final : public Tower::Tower {
private:
    int m_cooldown = 120;       // 射擊冷卻時間（幀數）
    int m_currentCooldown = 0;  // 當前剩餘冷卻時間
    float m_triggerRadius = 0.0f;   // 炸彈觸發半徑
    float m_explosionRadius = 0.0f; // 炸彈爆炸半徑
    bool m_draggable = false;   // 是否可拖曳
    bool m_clickable = true;    // 是否可點擊
    Components::CollisionComponent m_collision;

public:
    Cannon(const Util::PTSDPosition &position, float range = RANGE_BOMB);
    ~Cannon() = default;

    // 處理射程內氣球的方法 - 發射炸彈
    void handleBloonsInRange(const std::vector<std::shared_ptr<Bloon>> &bloons,
                             const std::vector<float> &distances) override;

    // I_draggable 介面實現
    void onDragStart() override { LOG_DEBUG("CANNON: Dragging start"); }
    void onDrag(const Util::PTSDPosition &newPosition) override {
        m_body->setPosition(newPosition);
        m_range->setPosition(newPosition);
        m_collision.setPosition(newPosition);
    }
    void onDragEnd() override { LOG_DEBUG("CANNON: Dragging end"); }
    bool isDraggable() const override { return m_draggable; }
    void setDraggable(bool draggable) override { m_draggable = draggable; }

    // CollisionComponent 的實現
    std::shared_ptr<Components::CollisionComponent> getCollisionComponent() const override {
        return m_range->getCollisionComponent();
    }
    void setSpecConfig(std::shared_ptr<SpecConfig> specConfig) override {
        auto bombConfig = std::dynamic_pointer_cast<BombSpecConfig>(specConfig);
        if (bombConfig) {
            m_triggerRadius = bombConfig->triggerRadius;
            m_explosionRadius = bombConfig->explosionRadius;
        }
    }
};

#endif // CANNON_HPP