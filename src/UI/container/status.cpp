#include "UI/container/status.hpp"
#include "Util/Logger.hpp"

namespace UI {

StatusItem::StatusItem(const std::string &title,
                       const std::string &initialValue, float width,
                       float height, float zIndex)
    : GameObject(nullptr, zIndex), m_width(width), m_height(height) {

  // 創建背景 (完全透明)
  bgShape = std::make_shared<Util::Shape>(Util::ShapeType::Rectangle,
                                          glm::vec2(width, height));
  bgShape->SetColorRGB(50, 50, 50, 0); // 完全透明
  m_background = std::make_shared<Util::GameObject>(bgShape, zIndex);
  AddChild(m_background);

  // 創建標題文字 (左側)
  m_titleText =
      std::make_shared<Util::Text>(RESOURCE_DIR "/font/Trebuchet MS.ttf", 18,
                                   title, Util::Color(255, 255, 255));
  m_titleObj = std::make_shared<Util::GameObject>(m_titleText, zIndex + 0.1f);
  AddChild(m_titleObj);

  // 創建數值文字 (右側)
  m_valueText = std::make_shared<Util::Text>(
      RESOURCE_DIR "/font/Trebuchet MS.ttf", 18, initialValue,
      Util::Color(255, 255, 0) // 黃色
  );
  m_valueObj = std::make_shared<Util::GameObject>(m_valueText, zIndex + 0.1f);
  AddChild(m_valueObj);

  LOG_DEBUG("StatusItem: Created with title '{}' and value '{}'", title,
            initialValue);
}

void StatusItem::updateValue(const std::string &value) {
  if (m_valueText) {
    m_valueText->SetText(value);
    LOG_DEBUG("StatusItem: Value updated to '{}'", value);
  }
}

void StatusItem::setSize(float width, float height) {
  m_width = width;
  m_height = height;

  // 更新背景大小
  auto shape = std::dynamic_pointer_cast<Util::Shape>(bgShape);
  if (shape) {
    shape->SetSize(glm::vec2(width, height));
  }

  // 使用 updateTextPositions 設置正確位置
  updateTextPositions();

  LOG_DEBUG("StatusItem: Size updated to {}x{}", width, height);
}

void StatusItem::setPosition(const Util::PTSDPosition &position) {
  // 更新本身位置
  m_Transform.translation = position.ToVec2();

  // 更新子元素的絕對位置
  updateTextPositions();
}

void StatusItem::updateTextPositions() {
  // 獲取當前絕對位置
  glm::vec2 myPosition = m_Transform.translation;

  // 設置子元素絕對位置
  m_titleObj->m_Transform.translation =
      glm::vec2(myPosition.x - m_width / 4, myPosition.y);
  m_valueObj->m_Transform.translation =
      glm::vec2(myPosition.x + m_width / 4, myPosition.y);

  LOG_DEBUG("StatusItem: Text positions updated at ({}, {})", myPosition.x,
            myPosition.y);
}

StatusBar::StatusBar(const Util::PTSDPosition &position, float width,
                     float height, float zIndex, bool fixedSize)
    : GameObject(nullptr, zIndex), m_width(width), m_height(height),
      m_padding(10.0f), m_spacing(5.0f) {

  // 創建背景
  m_backgroundShape = std::make_shared<Util::Shape>(Util::ShapeType::Rectangle,
                                                    glm::vec2(width, height));
  m_backgroundShape->SetColorRGB(30, 30, 30, 200); // 半透明深灰色
  SetDrawable(m_backgroundShape);

  // 設置位置
  m_Transform.translation = position.ToVec2();

  // 計算子項目尺寸
  float itemWidth = width - 2 * m_padding;
  float itemHeight =
      (height - 2 * m_padding - m_spacing) / 2.0f; // 兩個項目平分

  // 創建金錢項目
  m_moneyItem = std::make_shared<StatusItem>("Money:", "0", itemWidth,
                                             itemHeight, zIndex + 0.1f);
  AddChild(m_moneyItem);

  // 創建生命項目
  m_livesItem = std::make_shared<StatusItem>("Lives:", "0", itemWidth,
                                             itemHeight, zIndex + 0.1f);
  AddChild(m_livesItem);

  // 定位項目
  updateLayout();

  LOG_INFO("StatusBar: Created with dimensions {}x{} at position ({}, {})",
           width, height, position.x, position.y);
}

void StatusBar::updateMoney(int amount) {
  if (m_moneyItem) {
    m_moneyItem->updateValue(std::to_string(amount));
  }
}

void StatusBar::updateLives(int amount) {
  if (m_livesItem) {
    m_livesItem->updateValue(std::to_string(amount));
  }
}

void StatusBar::setPosition(const Util::PTSDPosition &position) {
  // 更新本身位置
  m_Transform.translation = position.ToVec2();

  // 更新子元素的絕對位置
  updateLayout();
}

void StatusBar::setSize(const glm::vec2 &size) {
  m_width = size.x;
  m_height = size.y;

  // 更新背景大小
  if (m_backgroundShape) {
    m_backgroundShape->SetSize(size);
  }

  // 更新子元素大小和位置
  float itemWidth = m_width - 2 * m_padding;
  float itemHeight = (m_height - 2 * m_padding - m_spacing) / 2.0f;

  if (m_moneyItem) {
    m_moneyItem->setSize(itemWidth, itemHeight);
  }

  if (m_livesItem) {
    m_livesItem->setSize(itemWidth, itemHeight);
  }

  // 重新定位子元素
  updateLayout();

  LOG_DEBUG("StatusBar: Size updated to {}x{}", m_width, m_height);
}

void StatusBar::setPadding(float padding) {
  m_padding = padding;
  updateLayout();
}

void StatusBar::setSpacing(float spacing) {
  m_spacing = spacing;
  updateLayout();
}

void StatusBar::updateLayout() {
  if (!m_moneyItem || !m_livesItem)
    return;

  // 獲取StatusBar的絕對位置（相對於畫面中心）
  glm::vec2 barPosition = m_Transform.translation;

  // 計算項目的絕對位置（相對於畫面中心）
  float moneyPosY =
      barPosition.y + (m_height / 2 - m_padding - m_moneyItem->getSize().y / 2);
  float livesPosY = moneyPosY - m_moneyItem->getSize().y - m_spacing;

  // 設置金錢項位置（絕對座標）
  m_moneyItem->setPosition(Util::PTSDPosition(barPosition.x, moneyPosY));

  // 設置生命項位置（絕對座標）
  m_livesItem->setPosition(Util::PTSDPosition(barPosition.x, livesPosY));

  // 確保文字位置正確更新
  m_moneyItem->updateTextPositions();
  m_livesItem->updateTextPositions();

  LOG_DEBUG(
      "StatusBar: Layout updated - Money at global y={}, Lives at global y={}",
      moneyPosY, livesPosY);
}

} // namespace UI