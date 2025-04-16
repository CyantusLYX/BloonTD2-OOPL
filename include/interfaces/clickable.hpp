#ifndef I_CLICKABLE_HPP
#define I_CLICKABLE_HPP

namespace Interface {

class I_clickable {
public:
    virtual ~I_clickable() = default;
    
    virtual void onClick() = 0;
    virtual void onFocus() = 0;
    
    virtual bool isClickable() const = 0;
    virtual void setClickable(bool clickable) = 0;
};

} // namespace Interface
#endif