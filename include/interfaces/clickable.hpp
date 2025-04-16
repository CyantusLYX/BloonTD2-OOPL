#ifndef CLICKABLE_HPP
#define CLICKABLE_HPP
namespace Interfcace {
namespace Components {
namespace Interaction {

class I_clickable {
public:
    virtual ~I_clickable() = default;
    
    virtual void onClick() = 0;
    virtual void onFocus() = 0;
    
    virtual bool isClickable() const = 0;
    virtual void setClickable(bool clickable) = 0;
};

} // namespace Interaction
} // namespace Components
} // namespace Interfcace
#endif // ICLICKABLE_HPP