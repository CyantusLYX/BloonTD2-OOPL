#ifndef MOVE_HPP
#define MOVE_HPP
#include "Util/Position.hpp"
namespace Interface{
class I_move{
public:
  virtual void move() = 0;
  virtual Util::PTSDPosition next_position(int frames=1) = 0;
  virtual ~I_move() = default;
};
}
#endif