#ifndef TEST_HPP
#define TEST_HPP
#include <variant>
#include <Util/Position.hpp>
#include <Util/GameObject.hpp>
class coshader : public Util::GameObject{
public:
  coshader(const std::shared_ptr<Core::Drawable> &drawable,
          const float zIndex, const Util::PTSDPosition &pos = {0, 0},
          const std::variant<glm::vec2, int> circle_r_or_rectangle_xy_variant = 0,
          const bool visible = true,
          const std::vector<std::shared_ptr<GameObject>> &children =
              std::vector<std::shared_ptr<GameObject>>())
      : Util::GameObject(drawable, zIndex, {0, 0}, visible, children) {
    this->m_Transform.translation = pos.ToVec2();
  }
};
#endif