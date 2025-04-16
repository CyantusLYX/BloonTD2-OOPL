/* #ifndef SPIKE_HPP
#define SPIKE_HPP
#include "collapsible.hpp"
#include "popper.hpp"
#include <Util/Image.hpp>
#include <Util/Position.hpp>
class spike : public popper, public Collapsible { 
public:
  spike(const Util::PTSDPosition &pos = {0, 0})
      : Collapsible(
            std::make_shared<Util::Image>(RESOURCE_DIR "/poppers/spike.png"), 2,
            pos, 10) {
              life=10;

            }
};
#endif */