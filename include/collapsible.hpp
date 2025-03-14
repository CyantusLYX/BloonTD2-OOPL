#ifndef COLLAPSIBLE_HPP
#define COLLAPSIBLE_HPP
#include "Util/GameObject.hpp"
#include <glm/fwd.hpp>

class Collapsible : public Util::GameObject {
public:
  Collapsible(const std::shared_ptr<Core::Drawable> &drawable,
              const float zIndex, const glm::vec2 &pivot = {0, 0},
              const bool visible = true,
              const std::vector<std::shared_ptr<GameObject>> &children =
                  std::vector<std::shared_ptr<GameObject>>())
      : Util::GameObject(drawable, zIndex, pivot, visible, children) {}
    Collapsible() = default;
  bool isCollide(const Collapsible &other) const;
  bool isCollide(const glm::vec2 pt) const;
  virtual ~Collapsible() = default;
};

#endif // COLLAPSIBLE_HPP