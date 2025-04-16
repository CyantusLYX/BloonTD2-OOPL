#ifndef DRAGGABLE_HPP
#define DRAGGABLE_HPP

#include "Util/Position.hpp"
namespace Interface {
namespace Components {
namespace Interaction {

class I_draggable {
public:
    virtual ~I_draggable() = default;
    
    virtual void onDragStart() = 0;
    virtual void onDrag(const Util::PTSDPosition& newPosition) = 0;
    virtual void onDragEnd() = 0;
    
    virtual bool isDraggable() const = 0;
    virtual void setDraggable(bool draggable) = 0;
};

} // namespace Interaction
} // namespace Components
} // namespace Interface
#endif