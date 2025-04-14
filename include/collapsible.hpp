#ifndef COLLAPSIBLE_HPP
#define COLLAPSIBLE_HPP
#include "Util/GameObject.hpp"
#include <variant>
#include "Util/Position.hpp"

enum class ColType { OVAL, RECTANGLE };

class Collapsible : public Util::GameObject {
private:
  bool can_click = false;
  bool can_drag = false;

public:
  Collapsible(const std::shared_ptr<Core::Drawable> &drawable,
              const float zIndex, const Util::PTSDPosition &pos = {0, 0},
              const std::variant<glm::vec2, int>  circle_r_or_rectangle_xy_variant = 0, const bool visible = true,
              const std::vector<std::shared_ptr<GameObject>> &children =
                  std::vector<std::shared_ptr<GameObject>>())
      : Util::GameObject(drawable, zIndex, {0,0}, visible, children),
        m_col_parm(circle_r_or_rectangle_xy_variant) {
          this->m_Transform.translation = pos.ToVec2();
          m_col_type = circle_r_or_rectangle_xy_variant.index() == 1 ? ColType::OVAL : ColType::RECTANGLE;
        }

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

  void set_position(const Util::PTSDPosition &position);
  void set_col_parm(const std::variant<glm::vec2, int> &col_parm);
  static bool rec_to_oval(const Collapsible &rec, const Collapsible &oval) ;
  [[nodiscard]] static bool rec_to_rec(const Collapsible &rec1, const Collapsible &rec2) ;
  [[nodiscard]] static bool oval_to_oval(const Collapsible &oval1, const Collapsible &oval2) ;
  [[nodiscard]] bool isCollide(const Collapsible &that) const;
  [[nodiscard]] bool isCollide(Util::PTSDPosition pt) const;
  ~Collapsible() override = default;
  void set_can_click(const bool can_click) { this->can_click = can_click; }
  [[nodiscard]] bool get_can_click() const { return this->can_click; }
  [[nodiscard]] Util::PTSDPosition get_position()const;
  bool get_can_drag() const { return can_drag; }
  virtual void be_clicked(){};
  virtual void be_focus(){};
};

#endif // COLLAPSIBLE_HPP