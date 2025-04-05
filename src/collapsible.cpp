#include "collapsible.hpp"
#include "Util/Logger.hpp"
#include "Util/Position.hpp"
#include <glm/fwd.hpp>

// ColType to ColType
// wait to be implement
bool Collapsible::rec_to_oval(const Collapsible &rec,
                              const Collapsible &oval) const {
  if (rec.m_col_type != ColType::RECTANGLE ||
      oval.m_col_type != ColType::OVAL) {
    throw std::invalid_argument("err on collapsible::rec_to_oval");
  } else {
    glm::vec2 r = rec.get_position().ToVec2();
    glm::vec2 ur = {
        rec.get_position().x + std::get<glm::vec2>(rec.m_col_parm).x / 2,
        rec.get_position().y + std::get<glm::vec2>(rec.m_col_parm).y / 2};
    glm::vec2 dl = {
        rec.get_position().x - std::get<glm::vec2>(rec.m_col_parm).x / 2,
        rec.get_position().y - std::get<glm::vec2>(rec.m_col_parm).y / 2};
    glm::vec2 c = oval.get_position().ToVec2();
    int cr = std::get<int>(oval.m_col_parm);
    int min_x = std::min(abs(dl.x - c.x), abs(ur.x - c.x));
    int min_y = std::min(abs(dl.y - c.y), abs(ur.y - c.y));
    return min_x * min_x + min_y * min_y < cr * cr ||
           ((abs(r.x - c.x) < abs(ur.x - dl.x) / 2 + cr) &&
            abs(c.y - r.y) < abs(ur.y - dl.y) / 2) ||
           ((abs(r.x - c.x) < abs(ur.x - dl.x) / 2) &&
            abs(c.y - r.y) < abs(ur.y - dl.y) / 2 + cr);
  }
  return false;
}

bool Collapsible::rec_to_rec(const Collapsible &rec1,
                             const Collapsible &rec2) const {
  if (rec1.m_col_type != ColType::RECTANGLE ||
      rec2.m_col_type != ColType::RECTANGLE) {
    throw std::invalid_argument("err on collapsible::rec_to_rec");
  } else {
    return abs(rec1.get_position().x - rec2.get_position().x) <=
               abs(std::get<glm::vec2>(rec1.m_col_parm).x -
                   std::get<glm::vec2>(rec2.m_col_parm).x) /
                   2 &&
           abs(rec1.get_position().y - rec2.get_position().y) <=
               abs(std::get<glm::vec2>(rec1.m_col_parm).y -
                   std::get<glm::vec2>(rec2.m_col_parm).y) /
                   2;
  }
  return false;
}

bool Collapsible::oval_to_oval(const Collapsible &oval1,
                               const Collapsible &oval2) const {
  if (oval1.m_col_type != ColType::OVAL || oval2.m_col_type != ColType::OVAL) {
    throw std::invalid_argument("err on collapsible::oval_to_oval");
  } else {
    return (pow(oval1.get_position().x - oval2.get_position().x, 2) +
            pow(oval1.get_position().y - oval2.get_position().y, 2)) <
           pow(std::get<int>(oval1.m_col_parm) +
                   std::get<int>(oval2.m_col_parm),
               2);
  }
  return false;
}

bool Collapsible::isCollide(const Collapsible &that) const {
  // return false;

  switch (m_col_type) {
  case ColType::OVAL:
    switch (that.m_col_type) {
    case ColType::OVAL:
      return oval_to_oval(*this, that);
    case ColType::RECTANGLE:
      return rec_to_oval(that, *this);
    default:
      throw std::invalid_argument("err on collapsible::isCollide switch");
      return false;
    }
    return false;
  case ColType::RECTANGLE:
    switch (that.m_col_type) {
    case ColType::OVAL:
      return rec_to_oval(*this, that);
    case ColType::RECTANGLE:
      return rec_to_rec(*this, that);
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
bool Collapsible::isCollide(const Util::PTSDPosition pt) const {
  // return false;
  switch (m_col_type) {
  case ColType::OVAL:
    LOG_INFO("object({},{}) collide with point: ({}, {})", get_position().x,
             get_position().y, pt.x, pt.y);
    return (pow(pt.x - get_position().x, 2) + pow(pt.y - get_position().y, 2)) <
           pow(std::get<int>(m_col_parm), 2);
    LOG_INFO("not collide with point: ({}, {})", pt.x, pt.y);
    return false;
  case ColType::RECTANGLE:
    return (pt.x < get_position().x + std::get<glm::vec2>(m_col_parm).x / 2) &&
           (pt.x > get_position().x - std::get<glm::vec2>(m_col_parm).x / 2) &&
           (pt.y < get_position().y + std::get<glm::vec2>(m_col_parm).y / 2) &&
           (pt.y > get_position().y - std::get<glm::vec2>(m_col_parm).y / 2);
  default:
    throw std::invalid_argument("err on collapsible::isCollide switch");
    return false;
  }
}

void Collapsible::set_position(const Util::PTSDPosition &position) {
  m_Transform.translation = glm::vec2(position.x,position.y);
}

Util::PTSDPosition Collapsible::get_position() const {
  // return m_Transform.translation + m_Pivot;
  return Util::PTSDPosition(m_Transform.translation.x,
                            m_Transform.translation.y);
}