#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "Util/Position.hpp"

namespace Components {
namespace Interface {
enum class ColType { OVAL, RECTANGLE };

class I_collider {
public:
    virtual ~I_collider() = default;
    
    virtual bool isCollide(const I_collider& other) const = 0;
    virtual bool isCollidePoint(const Util::PTSDPosition& point) const = 0;
    
    virtual ColType getColType() const = 0;
    virtual Util::PTSDPosition getPosition() const = 0;
};

} // namespace Components
} // namespace Interface
#endif // COLLISION_HPP