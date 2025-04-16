#ifndef COLLISIONCOMP_HPP
#define COLLISIONCOMP_HPP

#include "Util/GameObject.hpp"
#include "interfaces/collision.hpp"
#include "Util/Position.hpp"
#include <variant>
#include <glm/glm.hpp>

namespace Components {

class CollisionComponent : public Interface::I_collider {
protected:
    std::variant<glm::vec2, int> m_colParam;
    Interface::ColType m_colType;
    Util::PTSDPosition m_position;

public:
    CollisionComponent(
        const Util::PTSDPosition& position,
        const std::variant<glm::vec2, int>& colParam
    );
    ~CollisionComponent() override = default;

    // I_collider 接口實現
    bool isCollide(const I_collider& other) const override;
    bool isCollide(const Util::PTSDPosition& point) const override;
    
    Interface::ColType getColType() const override { return m_colType; }
    virtual Util::PTSDPosition getPosition() const override { return m_position; }
    virtual void setPosition(const Util::PTSDPosition& position) override { m_position = position; }
    
    const std::variant<glm::vec2, int>& getColParam() const override { return m_colParam; }
    void setColParam(const std::variant<glm::vec2, int>& colParam) override;
    
    // 碰撞檢測輔助方法
    static bool recToOval(const CollisionComponent& rec, const CollisionComponent& oval);
    static bool recToRec(const CollisionComponent& rec1, const CollisionComponent& rec2);
    static bool ovalToOval(const CollisionComponent& oval1, const CollisionComponent& oval2);
};

} // namespace Components
#endif // COLLISIONCOMPONENT_HPP