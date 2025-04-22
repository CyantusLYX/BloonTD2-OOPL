#include "UI/container/container.hpp"
#include "Util/Logger.hpp"
#include "core/shape.hpp"
#include "interfaces/collision.hpp"
#include <algorithm>
#include <cmath>
#include <limits>

namespace UI {

bool isObjectVisible(const std::shared_ptr<Util::GameObject> &obj) {
  if (!obj)
    return false;
  bool isScaleZero =
      (obj->m_Transform.scale.x == 0.0f && obj->m_Transform.scale.y == 0.0f);
  if (isScaleZero) {
    return false;
  }
  // 假定可見（保守方法）
  return true;
}

UIContainer::UIContainer(const Util::PTSDPosition &position,
                         const glm::vec2 &size, float zIndex, bool visible)
    : Util::GameObject(
          std::make_shared<Util::Shape>(Util::ShapeType::Rectangle, size),
          zIndex, {0, 0}, visible),
      Components::CollisionComponent(position, size) {

  // 設置初始位置
  m_Transform.translation = position.ToVec2();

  // 設置形狀顏色（半透明）
  auto shape = std::dynamic_pointer_cast<Util::Shape>(m_Drawable);
  if (shape) {
    shape->SetColorRGB(200, 200, 200, 128); // 半透明灰色
  }
}

void UIContainer::addChild(const std::shared_ptr<Util::GameObject> &child,
                           bool updateLayout) {
  AddChild(child);
  if (updateLayout) {
    this->updateLayout();
  }
}

void UIContainer::removeChild(const std::shared_ptr<Util::GameObject> &child,
                              bool updateLayout) {
  RemoveChild(child);
  if (updateLayout) {
    this->updateLayout();
  }
}

void UIContainer::clearChildren() { m_Children.clear(); }

Util::PTSDPosition UIContainer::getWorldPosition(
    const std::shared_ptr<Util::GameObject> &child) const {
  // 計算子節點在世界座標系中的位置
  glm::vec2 localPos = child->m_Transform.translation;

  // 考慮旋轉
  if (m_Transform.rotation != 0.0f) {
    float cos_r = cos(m_Transform.rotation);
    float sin_r = sin(m_Transform.rotation);

    float rotatedX = localPos.x * cos_r - localPos.y * sin_r;
    float rotatedY = localPos.x * sin_r + localPos.y * cos_r;

    localPos = glm::vec2(rotatedX, rotatedY);
  }

  // 考慮縮放然後平移
  glm::vec2 worldPos = localPos * m_Transform.scale + m_Transform.translation;

  return Util::PTSDPosition(worldPos.x, worldPos.y);
}

Util::PTSDPosition
UIContainer::toLocalPosition(const Util::PTSDPosition &worldPos) const {
  // 將世界座標轉換為容器內的局部座標
  glm::vec2 localPos =
      (worldPos.ToVec2() - m_Transform.translation) / m_Transform.scale;

  // 考慮旋轉（逆變換）
  if (m_Transform.rotation != 0.0f) {
    float cos_r = cos(-m_Transform.rotation);
    float sin_r = sin(-m_Transform.rotation);

    float rotatedX = localPos.x * cos_r - localPos.y * sin_r;
    float rotatedY = localPos.x * sin_r + localPos.y * cos_r;

    localPos = glm::vec2(rotatedX, rotatedY);
  }

  return Util::PTSDPosition(localPos.x, localPos.y);
}

void UIContainer::resizeToFitChildren() {
  if (m_Children.empty()) {
    return;
  }

  // 找出所有子節點的邊界
  float minX = std::numeric_limits<float>::max();
  float minY = std::numeric_limits<float>::max();
  float maxX = std::numeric_limits<float>::lowest();
  float maxY = std::numeric_limits<float>::lowest();

  for (const auto &child : m_Children) {
    glm::vec2 pos = child->m_Transform.translation;
    glm::vec2 size = child->GetScaledSize();

    minX = std::min(minX, pos.x - size.x / 2);
    minY = std::min(minY, pos.y - size.y / 2);
    maxX = std::max(maxX, pos.x + size.x / 2);
    maxY = std::max(maxY, pos.y + size.y / 2);
  }

  // 考慮內邊距
  minX -= m_padding;
  minY -= m_padding;
  maxX += m_padding;
  maxY += m_padding;

  // 計算新的大小
  glm::vec2 newSize = {maxX - minX, maxY - minY};

  // 更新大小
  setSize(newSize);
}

void UIContainer::updateLayout() {
  if (m_Children.empty()) {
    return;
  }

  // 獲取容器的大小
  auto shape = std::dynamic_pointer_cast<Util::Shape>(m_Drawable);
  glm::vec2 size = shape ? shape->GetSize() : glm::vec2(100, 100);

  // 從上到下垂直佈局
  float currentY = size.y / 2 - m_padding;

  for (const auto &child : m_Children) {
    glm::vec2 childSize = child->GetScaledSize();

    // 水平居中，垂直從上往下排列
    child->m_Transform.translation = glm::vec2(0, currentY - childSize.y / 2);

    // 更新下一個元素的 Y 位置
    currentY -= (childSize.y + m_spacing);
  }

  LOG_DEBUG("UICon : Layout updated with {} children", m_Children.size());
}

void UIContainer::setPosition(const Util::PTSDPosition &position) {
  // 更新 GameObject 的位置
  m_Transform.translation = position.ToVec2();

  // 更新碰撞組件的位置
  Components::CollisionComponent::setPosition(position);

  LOG_DEBUG("UICon : Position set to ({}, {})", position.x, position.y);
}

void UIContainer::setSize(const glm::vec2 &size) {
  // 更新形狀大小
  auto shape = std::dynamic_pointer_cast<Util::Shape>(m_Drawable);
  if (shape) {
    shape->SetSize(size);
  }

  // 更新碰撞組件大小
  if (m_colType == Interface::ColType::OVAL) {
    // 如果是圓形，設定為半徑
    setColParam(std::min(size.x, size.y) / 2.0f);
  } else {
    // 如果是矩形，設定為尺寸
    setColParam(size);
  }

  // 重新佈局子元素
  updateLayout();

  LOG_DEBUG("UICon : Size set to {}x{}", size.x, size.y);
}

bool UIContainer::containsPoint(const Util::PTSDPosition &point) const {
  // 首先檢查點是否在容器本身內部
  if (isCollide(point)) {
    return true;
  }

  // 然後檢查點是否在任何一個子元素內部
  for (const auto &child : m_Children) {
    // 嘗試將子元素轉換為可碰撞的接口
    auto collider = std::dynamic_pointer_cast<Interface::I_collider>(child);
    if (collider && collider->isCollide(point)) {
      return true;
    }

    // 如果子元素是 UIContainer，遞歸檢查
    auto container = std::dynamic_pointer_cast<UIContainer>(child);
    if (container && container->containsPoint(point)) {
      return true;
    }
  }

  return false;
}

std::shared_ptr<Util::GameObject>
UIContainer::findChildAt(const Util::PTSDPosition &point) const {
  // 優先檢查子元素（從前到後）
  for (auto it = m_Children.rbegin(); it != m_Children.rend(); ++it) {
    const auto &child = *it;

    if (!isObjectVisible(child)) {
      continue;
    }

    // 嘗試將子元素轉換為可碰撞的接口
    auto collider = std::dynamic_pointer_cast<Interface::I_collider>(child);
    if (collider && collider->isCollide(point)) {
      // 如果子元素是 UIContainer，遞歸檢查
      auto container = std::dynamic_pointer_cast<UIContainer>(child);
      if (container) {
        auto nestedChild = container->findChildAt(point);
        if (nestedChild) {
          return nestedChild;
        }
      }
      return child;
    }
  }

  // 如果點在容器內部但不在任何子元素內部，返回容器本身
  if (isCollide(point)) {
    try {
      return std::static_pointer_cast<Util::GameObject>(
          std::const_pointer_cast<UIContainer>(shared_from_this()));
    } catch (const std::bad_weak_ptr &) {
      LOG_ERROR("UICon : Container must be created with std::make_shared");
      return nullptr;
    }
  }

  return nullptr;
}

void UIContainer::onClick() {
  LOG_DEBUG("UICon : Container clicked");

  // 可以在這裡添加容器被點擊時的額外邏輯
}

} // namespace UI