#include "collapsible.hpp"

// ColType to ColType
// wait to be implement
bool Collapsible::rec_to_oval(Collapsible &rec, Collapsible &oval) {
  if (rec.m_col_type != ColType::RECTANGLE ||
      oval.m_col_type != ColType::OVAL) {
    throw std::invalid_argument("err on collapsible::rec_to_oval");
  }
//   if ()
}
bool Collapsible::rec_to_rec(Collapsible &rec1, Collapsible &rec2) {
  return false;
}
bool Collapsible::oval_to_oval(Collapsible &oval1, Collapsible &oval2) {
  return false;
}

bool Collapsible::isCollide(const Collapsible &other) const {
  // return false;

  switch (m_col_type) {
  case ColType::OVAL:
    switch (Collapsible::m_col_type) {
    case ColType::OVAL:
      return false;
    case ColType::RECTANGLE:
      return false;
    default:
      throw std::invalid_argument("err on collapsible::isCollide switch");
      return false;
    }
    return false;
  case ColType::RECTANGLE:
    switch (Collapsible::m_col_type) {
    case ColType::OVAL:
      return false;
    case ColType::RECTANGLE:
      return false;
    default:
      throw std::invalid_argument("err on collapsible::isCollide switch");
      return false;
    }
    return false;
  default:
    throw std::invalid_argument("err on collapsible::isCollide switch");
    return false;
  }
}

// ColType to dot
// if it trigger every object's isCollide when mouse click
// it will be so odd, doesn't it?
bool Collapsible::isCollide(const glm::vec2 pt) const {
  // return false;
  switch (m_col_type) {
  case ColType::OVAL:
    return pow((pt.x - m_Pivot.x) / m_col_parm.x, 2) +
               pow((pt.y - m_Pivot.y) / m_col_parm.y, 2) >
           1.0;
  case ColType::RECTANGLE:
    return (pt.x < m_Pivot.x + m_col_parm.x / 2) &&
           (pt.x > m_Pivot.x - m_col_parm.x / 2) &&
           (pt.y < m_Pivot.y + m_col_parm.y / 2) &&
           (pt.y > m_Pivot.y - m_col_parm.y / 2);
  default:
    throw std::invalid_argument("err on collapsible::isCollide switch");
    return false;
  }
}