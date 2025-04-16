#ifndef COLLISION_COMP_HPP
#define COLLISION_COMP_HPP

#include "interfaces/collision.hpp"
#include "Util/Position.hpp"
#include <variant>
#include <glm/glm.hpp>

namespace Components {
using namespace Interface;
class CollisionComponent : public I_collider {
private:
    std::variant<glm::vec2, int> m_colParam;
    ColType m_colType;
    Util::PTSDPosition m_position;

public:
    CollisionComponent(
        const Util::PTSDPosition& position,
        const std::variant<glm::vec2, int>& colParam
    ) : m_colParam(colParam), m_position(position) {
        m_colType = colParam.index() == 1 ? ColType::OVAL : ColType::RECTANGLE;
    }

    // I_collider interface implementation
    bool isCollide(const I_collider& other) const override;
    bool isCollidePoint(const Util::PTSDPosition& point) const override;
    
    ColType getColType() const override { return m_colType; }
    Util::PTSDPosition getPosition() const override { return m_position; }
    
    // 碰撞参数
    const std::variant<glm::vec2, int>& getColParam() const { return m_colParam; }
    void setColParam(const std::variant<glm::vec2, int>& colParam);
    
    // 位置设置
    void setPosition(const Util::PTSDPosition& position) { m_position = position; }

    // 原先 Collapsible 中的碰撞检测方法
    static bool recToOval(const CollisionComponent& rec, const CollisionComponent& oval);
    static bool recToRec(const CollisionComponent& rec1, const CollisionComponent& rec2);
    static bool ovalToOval(const CollisionComponent& oval1, const CollisionComponent& oval2);
};

} // namespace Components
#endif 