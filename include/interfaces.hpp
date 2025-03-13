#ifndef INTERFACES_HPP
#define INTERFACES_HPP
#include <Util/Input.hpp>
namespace Interface {
class IUpdatable {
public:
    virtual void update() = 0;
    virtual ~IUpdatable() = default;
};
class ICollapsible {
public:
    virtual bool isCollide(const ICollapsible &other) const = 0;
    virtual ~ICollapsible() = default;
private:
    Util::PTSDPosition m_Position;
};
}

#endif // INTERFACES_HPP