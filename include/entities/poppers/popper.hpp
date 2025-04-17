#ifndef POPPERS_POPPER_HPP
#define POPPERS_POPPER_HPP
#include "Util/GameObject.hpp"
#include "Util/Input.hpp"
#include "components/collisionComp.hpp"
#include "entities/bloon.hpp"
#include "interfaces/interfaces.hpp"
#include <memory>
#include <variant>
#include <vector>
// A base object can pop bloons.
class popper : public Mortal, public Components::CollisionComponent {
protected:
  bool explosive = false;
public:
  popper(const Util::PTSDPosition &pos = {0, 0}, const std::variant<glm::vec2, float> &circle_r_or_rectangle_xy = 0.0f)
      : Components::CollisionComponent(pos, circle_r_or_rectangle_xy) {
    m_colType = circle_r_or_rectangle_xy.index() == 1 ? Interface::ColType::OVAL : Interface::ColType::RECTANGLE;}  
  bool is_explosive() const { return explosive; }
  virtual std::vector<bool> hit(std::vector<std::shared_ptr<Bloon>> bloons) = 0;
  virtual std::shared_ptr<Util::GameObject> get_object(){return nullptr;};
  virtual Util::PTSDPosition get_position() const=0;
};
#endif