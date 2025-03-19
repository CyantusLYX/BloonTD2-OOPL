#ifndef COLLAPSIBLE_HPP
#define COLLAPSIBLE_HPP
#include "Util/GameObject.hpp"

enum class ColType {OVAL, RECTANGLE};

class Collapsible : public Util::GameObject {
public:

  Collapsible(const std::shared_ptr<Core::Drawable> &drawable,
              const float zIndex, const glm::vec2 &pivot = {0, 0},
              const float circle_r = 0.0,
              const bool visible = true,
              const std::vector<std::shared_ptr<GameObject>> &children =
                    std::vector<std::shared_ptr<GameObject>>()
                    )
      : Util::GameObject(drawable, zIndex, pivot, visible, children), 
        m_col_parm(circle_r), m_col_type(ColType::OVAL){}

  Collapsible(const std::shared_ptr<Core::Drawable> &drawable,
              const float zIndex, const glm::vec2 &pivot = {0, 0},
              const glm::vec2 &rectangle_xy = {0, 0},
              const bool visible = true,
              const std::vector<std::shared_ptr<GameObject>> &children =
                    std::vector<std::shared_ptr<GameObject>>()
                    )
      : Util::GameObject(drawable, zIndex, pivot, visible, children), 
        m_col_parm(rectangle_xy), m_col_type(ColType::RECTANGLE){}
        
  Collapsible() = default;
  
  /**
  * @param m_col_parm The parameter of the collision box.
  *               If m_col_type == OVAL       then m_col_parm refers to the width and height of the oval
  *               If m_col_type == RECTANGLE  then m_col_parm refers to the width and height of the rectangle
  * @param m_col_type The type of the collision box        
  */
  glm::vec2 m_col_parm;
  ColType   m_col_type;

	bool rec_to_oval(Collapsible &rec, Collapsible &oval);
	bool rec_to_rec(Collapsible &rec1, Collapsible &rec2);
	bool oval_to_oval(Collapsible &oval1, Collapsible &oval2);
  bool isCollide(const Collapsible &other) const;
  bool isCollide(const glm::vec2 pt) const;
  virtual ~Collapsible() = default;
};

#endif // COLLAPSIBLE_HPP