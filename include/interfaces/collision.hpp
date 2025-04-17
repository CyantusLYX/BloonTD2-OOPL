#ifndef I_COLLIDER_HPP
#define I_COLLIDER_HPP

#include "Util/Position.hpp"
#include <memory>
#include <variant>
#include <glm/glm.hpp>

namespace Interface {

enum class ColType { OVAL, RECTANGLE };

class I_collider {
public:
    virtual ~I_collider() = default;
    
    // 檢測與其他碰撞體的碰撞
    virtual bool isCollide(const I_collider& other) const = 0;
    
    // 檢測與點的碰撞
    virtual bool isCollide(const Util::PTSDPosition& point) const = 0;
    
    virtual ColType getColType() const = 0;
    virtual Util::PTSDPosition getPosition() const = 0;
    virtual void setPosition(const Util::PTSDPosition& position) = 0;
    
    // 獲取碰撞參數
virtual const std::variant<glm::vec2, float>& getColParam() const = 0;
virtual void setColParam(const std::variant<glm::vec2, float>& colParam) = 0;
};

} // namespace Components
#endif