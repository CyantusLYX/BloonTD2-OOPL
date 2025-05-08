#ifndef ICE_BALL_HPP
#define ICE_BALL_HPP

#include "Util/Logger.hpp"
#include "Util/Time.hpp"
#include "components/canBuy.hpp"
#include "components/collisionComp.hpp"
#include "entities/bloon.hpp"
#include "tower.hpp"
#include "conf.hpp"

class IceBall final : public Tower::Tower {
private:
    int m_cooldown = 80;       // 冰凍冷卻時間（幀數）
    int m_currentCooldown = 0; // 當前剩餘冷卻時間
    int m_freezeFrames = 60;   // 氣球凍結持續時間（幀數）
    bool m_draggable = false;  // 是否可拖曳
    bool m_clickable = true;   // 是否可點擊
    Components::CollisionComponent m_collision;
    bool m_canFreezeLead = false;  // 是否可以凍結鉛氣球
    bool m_canFreezeWhite = false; // 是否可以凍結白氣球

public:
    IceBall(const Util::PTSDPosition &position, float range = float(RANGE_ICE));
    ~IceBall() = default;

    // 處理射程內氣球的方法 - 直接凍結範圍內所有氣球
    void handleBloonsInRange(const std::vector<std::shared_ptr<Bloon>> &bloons,
                           const std::vector<float> &distances) override;

    // I_draggable 介面實現
    void onDragStart() override { LOG_DEBUG("ICBAL : Dragging start"); }
    void onDrag(const Util::PTSDPosition &newPosition) override {
        m_body->setPosition(newPosition);
        m_range->setPosition(newPosition);
        m_collision.setPosition(newPosition);
    }
    void onDragEnd() override { LOG_DEBUG("ICBAL : Dragging end"); }
    bool isDraggable() const override { return m_draggable; }
    void setDraggable(bool draggable) override { m_draggable = draggable; }

    // 設置可凍結特殊氣球的能力
    void setCanFreezeLead(bool canFreeze) { m_canFreezeLead = canFreeze; }
    void setCanFreezeWhite(bool canFreeze) { m_canFreezeWhite = canFreeze; }

public:
    IceBall(const IceBall &) = default;
    IceBall(IceBall &&) = delete;
    IceBall &operator=(const IceBall &) = default;
    IceBall &operator=(IceBall &&) = delete;
};

#endif