#ifndef MAP_HPP
#define MAP_HPP
#include "path.hpp"
#include <Util/GameObject.hpp>
#include <memory>
#include <vector>
class Map : public Util::GameObject {
public:
  Map(const std::shared_ptr<Core::Drawable> &drawable, const float zIndex,
      const std::shared_ptr<Path> &path, const bool visible = true,
      const std::vector<std::shared_ptr<GameObject>> &children =
          std::vector<std::shared_ptr<GameObject>>())
      : Util::GameObject(drawable, zIndex, glm::vec2{0, 0}, visible, children),
        m_Path(path) {
    this->m_Transform.translation = {-19, -8};
  }

private:
  std::shared_ptr<Path> m_Path;

public:
  std::shared_ptr<Path> get_path() { return m_Path; }
};
#endif // MAP_HPP
