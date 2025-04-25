#include "UI/container/towerBtns.hpp"
#include "Util/Logger.hpp"
#include <algorithm>

namespace UI {

TowerButtonsPanel::TowerButtonsPanel(
    const Util::PTSDPosition &position,
    float width,
    float height,
    float zIndex
) : UIContainer(position, glm::vec2(width, height), zIndex) {
    
    // 設置垂直排列
    setLayoutDirection(LayoutDirection::Vertical);
    setAlignment(Alignment::Start); // 上對齊
    setPadding(5.0f);
    setSpacing(5.0f);
    
    // 創建第一個按鈕行
    auto firstRow = createRow();
    m_rows.push_back(firstRow);
    addChild(firstRow);
}

std::shared_ptr<UIContainer> TowerButtonsPanel::createRow() {
    // 計算行容器的寬度
    float rowWidth = getContentSize().x;
    
    // 創建新的行容器
    auto row = std::make_shared<UIContainer>(
        Util::PTSDPosition(0, 0), // 位置會由父容器設置
        glm::vec2(rowWidth, m_buttonSize.y),
        GetZIndex() + 0.1f
    );
    
    // 設置行容器為水平排列
    row->setLayoutDirection(LayoutDirection::Horizontal);
    row->setAlignment(Alignment::Start); // 左對齊
    row->setPadding(0.0f);
    row->setSpacing(5.0f);
    
    return row;
}

void TowerButtonsPanel::addTowerButton(const std::shared_ptr<TowerButton> &button) {
    // 調整按鈕大小
    button->setSize(m_buttonSize);
    
    // 獲取當前的最後一行
    auto &lastRow = m_rows.back();
    
    // 檢查最後一行是否已滿
    if (lastRow->getChildren().size() >= m_buttonsPerRow) {
        // 創建新行
        auto newRow = createRow();
        m_rows.push_back(newRow);
        addChild(newRow);
        
        // 將按鈕添加到新行
        newRow->addChild(button);
    } else {
        // 將按鈕添加到現有行
        lastRow->addChild(button);
    }
    
    LOG_DEBUG("BTN_PANEL: 添加了一個新的塔按鈕，當前共有 {} 個按鈕", getAllButtons().size());
}

void TowerButtonsPanel::setButtonsPerRow(size_t count) {
    if (count == 0) {
        LOG_ERROR("BTN_PANEL: 每行按鈕數不能為零");
        return;
    }
    
    m_buttonsPerRow = count;
    rearrangeButtons();
}

void TowerButtonsPanel::setButtonSize(const glm::vec2 &size) {
    m_buttonSize = size;
    rearrangeButtons();
}

void TowerButtonsPanel::clearButtons() {
    // 清除所有行和按鈕
    for (auto &row : m_rows) {
        row->clearChildren();
    }
    clearChildren();
    m_rows.clear();
    
    // 重新創建第一行
    auto firstRow = createRow();
    m_rows.push_back(firstRow);
    addChild(firstRow);
    
    LOG_DEBUG("BTN_PANEL: 已清除所有按鈕");
}

std::vector<std::shared_ptr<TowerButton>> TowerButtonsPanel::getAllButtons() const {
    std::vector<std::shared_ptr<TowerButton>> buttons;
    
    // 從所有行收集按鈕
    for (const auto &row : m_rows) {
        for (const auto &child : row->getChildren()) {
            auto button = std::dynamic_pointer_cast<TowerButton>(child);
            if (button) {
                buttons.push_back(button);
            }
        }
    }
    
    return buttons;
}

void TowerButtonsPanel::rearrangeButtons() {
    // 收集所有按鈕
    auto buttons = getAllButtons();
    
    // 清除所有行和按鈕
    clearButtons();
    
    // 重新添加所有按鈕
    for (const auto &button : buttons) {
        addTowerButton(button);
    }
    
    LOG_DEBUG("BTN_PANEL: 重新排列了 {} 個按鈕", buttons.size());
}

void TowerButtonsPanel::updateLayout() {
    // 獲取容器尺寸
    float contentWidth = getContentSize().x;
    float contentHeight = getContentSize().y;
    
    // 計算按鈕布局參數
    int buttonCount = m_Children.size();
    if (buttonCount == 0) return;
    
    float buttonSpacing = getSpacing();
    float buttonSize = std::min((contentWidth - buttonSpacing * (2 - 1)) / 2, 
                             (contentHeight - buttonSpacing * std::ceil(buttonCount / 2.0f - 1)) / std::ceil(buttonCount / 2.0f));
    
    // 確保合理的按鈕大小
    buttonSize = std::max(buttonSize, 40.0f);
    
    // 計算實際佈局的起始點（容器左上角加上內邊距）
    float startX = -contentWidth/2 + buttonSize/2 + getPadding();
    float startY = -contentHeight/2 + buttonSize/2 + getPadding();
    
    // 布局按鈕
    for (int i = 0; i < buttonCount; i++) {
        int row = i / 2;
        int col = i % 2;
        
        float x = startX + col * (buttonSize + buttonSpacing);
        float y = startY + row * (buttonSize + buttonSpacing);
        
        // 計算按鈕的世界位置（相對於容器）
        Util::PTSDPosition position(x, y);
        Util::PTSDPosition worldPosition = getWorldPosition(m_Children[i]);
        
        // 更新按鈕位置和大小
        auto button = std::dynamic_pointer_cast<TowerButton>(m_Children[i]);
        if (button) {
            // 關鍵：使用正確的方法設置按鈕位置
            button->setPosition(worldPosition);
            button->setSize(glm::vec2(buttonSize, buttonSize));
            
            LOG_DEBUG("TowerPanel: Button {} positioned at world ({}, {}), local ({}, {})", 
                     button->getName(), worldPosition.x, worldPosition.y, x, y);
        }
    }
}

} // namespace UI