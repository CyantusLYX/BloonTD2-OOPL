#ifndef INTERFACES_HPP
#define INTERFACES_HPP
#include "components/mortal.hpp"
#include "move.hpp"
#include <Util/Input.hpp>
#include <vector>

namespace Interface {
class IUpdatable {
private:
  std::vector<std::shared_ptr<IUpdatable>> m_UpdatableChildren;

public:
  void add_child(std::shared_ptr<IUpdatable> child) {
    m_UpdatableChildren.push_back(child);
  }
  std::vector<std::shared_ptr<IUpdatable>> get_children() {
    return m_UpdatableChildren;
  }
  virtual void update() = 0;
  virtual ~IUpdatable() = default;
};
} // namespace Interface

#endif // INTERFACES_HPP
