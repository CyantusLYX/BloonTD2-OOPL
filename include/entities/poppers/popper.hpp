#ifndef POPPERS_POPPER_HPP
#define POPPERS_POPPER_HPP
#include "collapsible.hpp"
#include "interfaces/interfaces.hpp"
// A base object can pop bloons.
class popper : public Interface::IUpdatable, public Mortal, public Collapsible {
protected:
  bool explosive = false;
public:
  bool is_explosive() const { return explosive; }
  virtual bool hit() = 0;
};
#endif