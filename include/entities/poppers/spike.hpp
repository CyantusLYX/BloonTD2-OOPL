#ifndef SPIKE_HPP
#define SPIKE_HPP
#include "Util/GameObject.hpp"
#include "popper.hpp"
#include <Util/Image.hpp>
#include <Util/Position.hpp>
#include <memory>
#include <vector>
class spike : public popper {
private:
  std::shared_ptr<Util::GameObject> m_object;
  int life = 10;

public:
  spike(const Util::PTSDPosition &pos);
  std::vector<bool> hit(std::vector<std::shared_ptr<Bloon>> bloons) override;
  std::shared_ptr<Util::GameObject> get_object() override;
  Util::PTSDPosition get_position() const override {
    auto pos = m_object->m_Transform.translation;
    return Util::PTSDPosition(pos.x, pos.y);
  }
};
#endif