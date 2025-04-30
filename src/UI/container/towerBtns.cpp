#include "UI/container/towerBtns.hpp"
#include "Util/Logger.hpp"
#include <algorithm>

namespace UI {

TowerButtonsPanel::TowerButtonsPanel(
    const Util::PTSDPosition &position,
    float width,
    float height,
    float zIndex
) : GameObject(nullptr, zIndex), m_width(width), m_height(height) {
    
    // 創建背景形狀
    m_backgroundShape = std::make_shared<Util::Shape>(
        Util::ShapeType::Rectangle, 
        glm::vec2(width, height)
    );
    SetDrawable(m_backgroundShape);
    m_backgroundShape->SetColorRGB(50, 50, 50, 10); // 半透明深灰色
    // 設置位置 (相對於畫面中心)
    m_Transform.translation = position.ToVec2();
    
    LOG_DEBUG("TowerButtonsPanel: Created with dimensions {}x{} at position ({}, {})",
              width, height, position.x, position.y);
}

void TowerButtonsPanel::addTowerButton(const std::shared_ptr<TowerButton> &button) {
    if (!button) return;
    
    // 添加到按鈕列表
    m_buttons.push_back(button);
    
    // 添加為子物件
    AddChild(button);
    
    // 更新布局
    updateLayout();
    
    LOG_DEBUG("TowerButtonsPanel: Added a new button, total buttons: {}", m_buttons.size());
}

void TowerButtonsPanel::setButtonsPerRow(size_t count) {
    if (count == 0) {
        LOG_ERROR("TowerButtonsPanel: Buttons per row cannot be zero");
        return;
    }
    
    m_buttonsPerRow = count;
    updateLayout();
    
    LOG_DEBUG("TowerButtonsPanel: Set buttons per row to {}", count);
}

void TowerButtonsPanel::setButtonSize(const glm::vec2 &size) {
    m_buttonSize = size;
    updateLayout();
    
    LOG_DEBUG("TowerButtonsPanel: Set button size to {}x{}", size.x, size.y);
}

void TowerButtonsPanel::clearButtons() {
    // 移除所有按鈕作為子物件
    for (const auto& button : m_buttons) {
        RemoveChild(button);
    }
    
    // 清空按鈕列表
    m_buttons.clear();
    
    LOG_DEBUG("TowerButtonsPanel: Cleared all buttons");
}

std::vector<std::shared_ptr<TowerButton>> TowerButtonsPanel::getAllButtons() const {
    return m_buttons;
}

void TowerButtonsPanel::setPosition(const Util::PTSDPosition &position) {
    // 更新本身位置
    m_Transform.translation = position.ToVec2();
    
    // 更新子元素位置
    updateLayout();
    
    LOG_DEBUG("TowerButtonsPanel: Position updated to ({}, {})", position.x, position.y);
}

void TowerButtonsPanel::setSize(const glm::vec2 &size) {
    m_width = size.x;
    m_height = size.y;
    
    // 更新背景形狀
    if (m_backgroundShape) {
        m_backgroundShape->SetSize(size);
    }
    
    // 更新按鈕位置
    updateLayout();
    
    LOG_DEBUG("TowerButtonsPanel: Size updated to {}x{}", size.x, size.y);
}

void TowerButtonsPanel::rearrangeButtons() {
    updateLayout();
}

void TowerButtonsPanel::updateLayout() {
    if (m_buttons.empty()) return;
    
    // 獲取面板絕對位置（相對於畫面中心）
    glm::vec2 panelPosition = m_Transform.translation;
    
    // 計算可用內容區域
    float contentWidth = m_width - 2 * m_padding;
    float contentHeight = m_height - 2 * m_padding;
    
    // 計算實際按鈕大小和間距
    float buttonSpacing = m_spacing;
    float maxButtonWidth = (contentWidth - buttonSpacing * (m_buttonsPerRow - 1)) / m_buttonsPerRow;
    float buttonSize = std::min(maxButtonWidth, m_buttonSize.x);  // 使用預設大小和最大可用大小的較小值
    
    // 計算按鈕行數
    size_t buttonCount = m_buttons.size();
    size_t rowCount = (buttonCount + m_buttonsPerRow - 1) / m_buttonsPerRow;  // 向上取整
    
    // 檢查是否需要調整按鈕大小以適應高度
    float totalRowsHeight = rowCount * buttonSize + (rowCount - 1) * buttonSpacing;
    if (totalRowsHeight > contentHeight) {
        // 按鈕太大，無法適應高度，需要調整大小
        float maxButtonHeight = (contentHeight - buttonSpacing * (rowCount - 1)) / rowCount;
        buttonSize = std::min(buttonSize, maxButtonHeight);
    }
    
    // 計算起始位置（左上角）
    float startX = panelPosition.x - m_width/2 + m_padding + buttonSize/2;
    float startY = panelPosition.y + m_height/2 - m_padding - buttonSize/2;
    
    // 布局按鈕
    for (size_t i = 0; i < m_buttons.size(); ++i) {
        // 計算行列位置
        size_t row = i / m_buttonsPerRow;
        size_t col = i % m_buttonsPerRow;
        
        // 計算按鈕中心位置
        float x = startX + col * (buttonSize + buttonSpacing);
        float y = startY - row * (buttonSize + buttonSpacing);
        
        // 設置按鈕位置（絕對座標）
        m_buttons[i]->setPosition(Util::PTSDPosition(x, y));
        
        // 設置按鈕大小
        m_buttons[i]->setSize(glm::vec2(buttonSize, buttonSize));
        
        LOG_DEBUG("TowerButtonsPanel: Button {} positioned at ({}, {})", 
                 m_buttons[i]->getName(), x, y);
    }
}

} // namespace UI