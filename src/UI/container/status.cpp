#include "UI/container/status.hpp"
#include "Util/Logger.hpp"
#include "core/shape.hpp"

namespace UI {

StatusItem::StatusItem(const std::string &title,
                       const std::string &initialValue, float width,
                       float height, float zIndex)
    : UIContainer(Util::PTSDPosition(0, 0), glm::vec2(width, height), zIndex) {
  // 設置容器透明
  auto shape = std::dynamic_pointer_cast<Util::Shape>(m_Drawable);
  if (shape) {
    shape->SetColorRGB(50, 50, 50, 0); // 完全透明
  }
  
  // 禁用自動大小調整
  setAutoSize(false);

  // 設置為水平排列
  setLayoutDirection(LayoutDirection::Horizontal);
  setPadding(0.0f);
  setSpacing(0.0f);

  // 計算子容器大小
  float containerWidth = width / 2.0f;
  float containerHeight = height;

  // 創建標題容器（左對齊）
  m_titleContainer = std::make_shared<UIContainer>(
      Util::PTSDPosition(0, 0), glm::vec2(containerWidth, containerHeight),
      zIndex + 0.1f);
  m_titleContainer->setLayoutDirection(LayoutDirection::Horizontal);
  m_titleContainer->setAlignment(Alignment::Start); // 左對齊
  m_titleContainer->setAutoSize(false); // 禁用自動大小調整

  // 創建值容器（右對齊）
  m_valueContainer = std::make_shared<UIContainer>(
      Util::PTSDPosition(0, 0), glm::vec2(containerWidth, containerHeight),
      zIndex + 0.1f);
  m_valueContainer->setLayoutDirection(LayoutDirection::Horizontal);
  m_valueContainer->setAlignment(Alignment::End); // 右對齊
  m_valueContainer->setAutoSize(false); // 禁用自動大小調整

  // 創建標題文字
  auto titleTextDrawable =
      std::make_shared<Util::Text>(RESOURCE_DIR "/font/Trebuchet MS.ttf", 18,
                                   title, Util::Color(255, 255, 255));
  m_titleObj =
      std::make_shared<Util::GameObject>(titleTextDrawable, zIndex + 0.2f);

  // 創建數值文字
  auto valueTextDrawable = std::make_shared<Util::Text>(
      RESOURCE_DIR "/font/Trebuchet MS.ttf", 18, initialValue,
      Util::Color(255, 255, 0)); // 黃色值
  m_valueObj =
      std::make_shared<Util::GameObject>(valueTextDrawable, zIndex + 0.2f);

  // 保存文字引用
  m_titleText = titleTextDrawable;
  m_valueText = valueTextDrawable;

  // 添加文字到對應容器
  m_titleContainer->addChild(m_titleObj);
  m_valueContainer->addChild(m_valueObj);

  // 添加容器到主容器
  addChild(m_titleContainer);
  addChild(m_valueContainer);

  // 初始更新佈局
  updateLayout();
}

void StatusItem::updateValue(const std::string &value) {
  if (m_valueText) {
    m_valueText->SetText(value);
  }
}

StatusBar::StatusBar(const Util::PTSDPosition &position, float width,
                     float height, float zIndex, bool fixedSize)
    : UIContainer(position, glm::vec2(width, height), zIndex) {
  // 設置容器背景
  auto shape = std::dynamic_pointer_cast<Util::Shape>(m_Drawable);
  if (shape) {
    shape->SetColorRGB(30, 30, 30, 200); // 半透明深灰色背景
  }

  // 自動大小調整
  setAutoSize(!fixedSize);

  // 設置為垂直排列
  setLayoutDirection(LayoutDirection::Vertical);
  setPadding(10.0f);
  setSpacing(5.0f);

  // 計算每個狀態項的尺寸
  float itemWidth = width - 2 * getPadding();
  float itemHeight =
      (height - 2 * getPadding() - getSpacing()) / 2.0f; // 兩個項目平分

  // 創建 Money 項
  m_moneyItem = std::make_shared<StatusItem>("Money:", "0", itemWidth,
                                             itemHeight, zIndex + 0.1f);

  // 創建 Lives 項
  m_livesItem = std::make_shared<StatusItem>("Lives:", "0", itemWidth,
                                             itemHeight, zIndex + 0.1f);

  // 添加到容器
  addChild(m_moneyItem);
  addChild(m_livesItem);

  LOG_DEBUG("StatusBar: Created with dimensions {}x{}", width, height);
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

} // namespace UI