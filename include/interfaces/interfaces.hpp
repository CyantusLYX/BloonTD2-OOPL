#ifndef INTERFACES_HPP
#define INTERFACES_HPP
#include <Util/Input.hpp>
#include "move.hpp"
#include "components/mortal.hpp"

namespace Interface {
class IUpdatable {
public:
    virtual void update() = 0;
    virtual ~IUpdatable() = default;
};
}

#endif // INTERFACES_HPP