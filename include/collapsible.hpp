#ifndef COLLAPSIBLE_HPP
#define COLLAPSIBLE_HPP
#include "Util/GameObject.hpp"
#include <variant>
#include "Util/Position.hpp"

enum class ColType { OVAL, RECTANGLE };

class Collapsible : public Util::GameObject {
private:
  bool can_click = false;

public:
  Collapsible(const std::shared_ptr<Core::Drawable> &drawable,
              const float zIndex, const glm::vec2 &pivot = {0, 0},
              const int circle_r = 0, const bool visible = true,
              const std::vector<std::shared_ptr<GameObject>> &children =
                  std::vector<std::shared_ptr<GameObject>>())
      : Util::GameObject(drawable, zIndex, pivot, visible, children),
        m_col_parm(circle_r), m_col_type(ColType::OVAL) {}

  Collapsible(const std::shared_ptr<Core::Drawable> &drawable,
              const float zIndex, const glm::vec2 &pivot = {0, 0},
              const glm::vec2 &rectangle_xy = {0, 0}, const bool visible = true,
              const std::vector<std::shared_ptr<GameObject>> &children =
                  std::vector<std::shared_ptr<GameObject>>())
      : Util::GameObject(drawable, zIndex, pivot, visible, children),
        m_col_parm(rectangle_xy), m_col_type(ColType::RECTANGLE) {}

  Collapsible() = default;

  /**
   * @param m_col_parm The parameter of the collision box.
   *               If m_col_type == OVAL       then m_col_parm refers to the
   * width and height of the oval If m_col_type == RECTANGLE  then m_col_parm
   * refers to the width and height of the rectangle
   * @param m_col_type The type of the collision box
   */
  std::variant<glm::vec2, int> m_col_parm;
  ColType m_col_type;

  void set_position(const glm::vec2 &position);
  bool rec_to_oval(const Collapsible &rec, const Collapsible &oval) const;
  bool rec_to_rec(const Collapsible &rec1, const Collapsible &rec2) const;
  bool oval_to_oval(const Collapsible &oval1, const Collapsible &oval2) const;
  bool isCollide(const Collapsible &that) const;
  bool isCollide(const glm::vec2 pt) const;
  virtual ~Collapsible() = default;
  void set_can_click(bool can_click) { this->can_click = can_click; }
  bool get_can_click() { return this->can_click; }
  Util::PTSDPosition get_position()const;
};

#endif // COLLAPSIBLE_HPP