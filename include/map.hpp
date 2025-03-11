#ifndef MAP_HPP
#define MAP_HPP
#include "pch.hpp"
#include <Util/GameObject.hpp>
#include <glm/fwd.hpp>
#include <vector>
class Map : public Util::GameObject {
public:
  Map(const std::shared_ptr<Core::Drawable> &drawable, const float zIndex,
      const bool visible = true,
      const std::vector<std::shared_ptr<GameObject>> &children =
          std::vector<std::shared_ptr<GameObject>>())
      : Util::GameObject(drawable, zIndex, glm::vec2{0, 0}, visible, children) {
    this->m_Transform.translation = {-19, -8};
  }
};
#endif // MAP_HPP