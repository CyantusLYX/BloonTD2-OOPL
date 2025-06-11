#ifndef UI_TOWER_BUTTONS_HPP
#define UI_TOWER_BUTTONS_HPP

#include "UI/buttons/tower_btn.hpp"
#include "Util/GameObject.hpp"
#include "Util/Position.hpp"
#include "core/shape.hpp"
#include <memory>
#include <vector>

namespace UI {

// 塔購買按鈕面板，自動管理並排列TowerButton，不再繼承UIContainer
class TowerButtonsPanel : public Util::GameObject {
private:
  // 背景形狀
  std::shared_ptr<Util::Shape> m_backgroundShape;

  // 按鈕相關設置
  size_t m_buttonsPerRow = 4; // 每行按鈕數量
  glm::vec2 m_buttonSize = {35.0f, 35.0f};

  // 所有按鈕
  std::vector<std::shared_ptr<TowerButton>> m_buttons;

  // 內部布局參數
  float m_width;
  float m_height;
  float m_padding = 2.0f;
  float m_spacing = 5.0f;

  // 重新排列所有按鈕
  void rearrangeButtons();

public:
  // 構造函數
  TowerButtonsPanel(const Util::PTSDPosition &position, float width,
                    float height, float zIndex = 10.0f);

  // 添加一個塔購買按鈕
  void addTowerButton(const std::shared_ptr<TowerButton> &button);

  // 設置每行按鈕數量
  void setButtonsPerRow(size_t count);

  // 設置按鈕大小
  void setButtonSize(const glm::vec2 &size);

  // 清除所有按鈕
  void clearButtons();

  // 獲取所有按鈕
  std::vector<std::shared_ptr<TowerButton>> getAllButtons() const;

  // 設置位置
  void setPosition(const Util::PTSDPosition &position);

  // 設置大小
  void setSize(const glm::vec2 &size);

  // 獲取大小
  glm::vec2 getSize() const { return {m_width, m_height}; }

  // 布局參數設置
  void setPadding(float padding) {
    m_padding = padding;
    updateLayout();
  }
  void setSpacing(float spacing) {
    m_spacing = spacing;
    updateLayout();
  }

  // 更新布局
  void updateLayout();

  // 獲取背景形狀
  std::shared_ptr<Util::Shape> getBackgroundShape() const {
    return m_backgroundShape;
  }
};

} // namespace UI

#endif // UI_TOWER_BUTTONS_HPP
