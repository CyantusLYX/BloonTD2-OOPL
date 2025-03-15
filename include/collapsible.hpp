#ifndef COLLAPSIBLE_HPP
#define COLLAPSIBLE_HPP
#include "Util/GameObject.hpp"
#include <glm/fwd.hpp>

enum class ColType {DOT, OVAL, RECTANGLE};
class Collapsible : public Util::GameObject {
public:
  Collapsible(const std::shared_ptr<Core::Drawable> &drawable,
              const float zIndex, const glm::vec2 &pivot = {0, 0},
              const glm::vec2 &pcol = {0, 0},
              const ColType tcol = ColType::RECTANGLE,
              const bool visible = true,
              const std::vector<std::shared_ptr<GameObject>> &children =
                    std::vector<std::shared_ptr<GameObject>>()
                    )
      : Util::GameObject(drawable, zIndex, pivot, visible, children), 
        m_pcol(pcol), m_tcol(tcol){}
  Collapsible() = default;
  
  /**
  * @param m_pcol The parameter of collide box.
  *               If m_tcol == DOT        then m_pcol should be pivot or just {0, 0}
  *               If m_tcol == OVAL       then m_pcol refers to {w, h} in (x/w)^2 + (y/h)^2 = 1
  *               If m_tcol == RECTANGLE  then m_pcol refers to {w, h} in x=(+-w/2), y=(+-h/2)
  * @param m_tcol The type of collide box        
  */
  glm::vec2 m_pcol;
  ColType   m_tcol;

  bool isCollide(const Collapsible &other) const;
  bool isCollide(const glm::vec2 pt) const;
  virtual ~Collapsible() = default;
};

#endif // COLLAPSIBLE_HPP