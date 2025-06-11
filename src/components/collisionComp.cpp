#include "components/collisionComp.hpp"
#include <cmath>

namespace Components {

CollisionComponent::CollisionComponent(
    const Util::PTSDPosition &position,
    const std::variant<glm::vec2, float> &colParam)
    : m_colParam(colParam),
      m_position(position) {
  m_colType = colParam.index() == 0 ? Interface::ColType::RECTANGLE
                                    : Interface::ColType::OVAL;
}

void CollisionComponent::setColParam(
    const std::variant<glm::vec2, float> &colParam) {
  if (m_colParam.index() == colParam.index())
    this->m_colParam = colParam;
}

bool CollisionComponent::recToOval(const CollisionComponent &rec,
                                   const CollisionComponent &oval) {
  if (rec.getColType() != Interface::ColType::RECTANGLE ||
      oval.getColType() != Interface::ColType::OVAL) {
    throw std::invalid_argument("err on CollisionComponent::recToOval");
  } else {
    const glm::vec2 r = rec.getPosition().ToVec2();
    const glm::vec2 &recSize = std::get<glm::vec2>(rec.getColParam());
    const glm::vec2 ur = {rec.getPosition().x + recSize.x / 2,
                          rec.getPosition().y + recSize.y / 2};
    const glm::vec2 dl = {rec.getPosition().x - recSize.x / 2,
                          rec.getPosition().y - recSize.y / 2};
    glm::vec2 c = oval.getPosition().ToVec2();
    int cr = std::get<float>(oval.getColParam());
    float min_x = std::min(std::fabs(dl.x - c.x), std::fabs(ur.x - c.x));
    float min_y = std::min(std::fabs(dl.y - c.y), std::fabs(ur.y - c.y));
    return min_x * min_x + min_y * min_y < cr * cr ||
           ((std::fabs(r.x - c.x) < std::fabs(ur.x - dl.x) / 2 + cr) &&
            std::fabs(c.y - r.y) < std::fabs(ur.y - dl.y) / 2) ||
           ((std::fabs(r.x - c.x) < std::fabs(ur.x - dl.x) / 2) &&
            std::fabs(c.y - r.y) < std::fabs(ur.y - dl.y) / 2 + cr);
  }
  return false;
}

bool CollisionComponent::recToRec(const CollisionComponent &rec1,
                                  const CollisionComponent &rec2) {
  if (rec1.getColType() != Interface::ColType::RECTANGLE ||
      rec2.getColType() != Interface::ColType::RECTANGLE) {
    throw std::invalid_argument("err on CollisionComponent::recToRec");
  } else {
    const glm::vec2 &rec1Size = std::get<glm::vec2>(rec1.getColParam());
    const glm::vec2 &rec2Size = std::get<glm::vec2>(rec2.getColParam());

    return std::fabs(rec1.getPosition().x - rec2.getPosition().x) <=
               (rec1Size.x + rec2Size.x) / 2 &&
           std::fabs(rec1.getPosition().y - rec2.getPosition().y) <=
               (rec1Size.y + rec2Size.y) / 2;
  }
  return false;
}

bool CollisionComponent::ovalToOval(const CollisionComponent &oval1,
                                    const CollisionComponent &oval2) {
  if (oval1.getColType() != Interface::ColType::OVAL ||
      oval2.getColType() != Interface::ColType::OVAL) {
    throw std::invalid_argument("err on CollisionComponent::ovalToOval");
  } else {
    int r1 = std::get<float>(oval1.getColParam());
    int r2 = std::get<float>(oval2.getColParam());

    return (pow(oval1.getPosition().x - oval2.getPosition().x, 2) +
            pow(oval1.getPosition().y - oval2.getPosition().y, 2)) <
           pow(r1 + r2, 2);
  }
  return false;
}

bool CollisionComponent::isCollide(const I_collider &other) const {
  const CollisionComponent *otherComponent =
      dynamic_cast<const CollisionComponent *>(&other);
  if (!otherComponent) {
    return false;
  }

  switch (m_colType) {
  case Interface::ColType::OVAL:
    switch (otherComponent->getColType()) {
    case Interface::ColType::OVAL:
      return ovalToOval(*this, *otherComponent);
    case Interface::ColType::RECTANGLE:
      return recToOval(*otherComponent, *this);
    default:
      throw std::invalid_argument(
          "err on CollisionComponent::isCollide switch");
    }
    break;

  case Interface::ColType::RECTANGLE:
    switch (otherComponent->getColType()) {
    case Interface::ColType::OVAL:
      return recToOval(*this, *otherComponent);
    case Interface::ColType::RECTANGLE:
      return recToRec(*this, *otherComponent);
    default:
      throw std::invalid_argument(
          "err on CollisionComponent::isCollide switch");
    }
    break;

  default:
    throw std::invalid_argument("err on CollisionComponent::isCollide switch");
  }

  return false;
}

bool CollisionComponent::isCollide(const Util::PTSDPosition &point) const {
  switch (m_colType) {
  case Interface::ColType::OVAL: {
    int radius = std::get<float>(m_colParam);
    return (pow(point.x - m_position.x, 2) + pow(point.y - m_position.y, 2)) <
           pow(radius, 2);
  }

  case Interface::ColType::RECTANGLE: {
    const glm::vec2 &size = std::get<glm::vec2>(m_colParam);
    return (point.x < m_position.x + size.x / 2) &&
           (point.x > m_position.x - size.x / 2) &&
           (point.y < m_position.y + size.y / 2) &&
           (point.y > m_position.y - size.y / 2);
  }

  default:
    throw std::invalid_argument(
        "err on CollisionComponent::isCollide point switch");
  }

  return false;
}

} // namespace Components
