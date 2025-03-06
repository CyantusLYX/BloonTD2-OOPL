#ifndef INTERFACES_HPP
#define INTERFACES_HPP
namespace Interface {
class IUpdatable {
public:
    virtual void update() = 0;
    virtual ~IUpdatable() = default;
};
}
#endif // INTERFACES_HPP