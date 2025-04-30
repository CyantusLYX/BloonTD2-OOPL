#ifndef UI_STATUS_HPP
#define UI_STATUS_HPP

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"
#include "Util/Position.hpp"
#include "core/shape.hpp"
#include "interfaces/clickable.hpp"
#include <memory>
#include <string>

namespace UI {

// 單一狀態項目類，用於顯示一個標題和它對應的數值
class StatusItem : public Util::GameObject {
private:
    std::shared_ptr<Util::Shape> bgShape; // 背景形狀
    std::shared_ptr<Util::GameObject> m_background; // 背景物件
    std::shared_ptr<Util::Text> m_titleText;  // 標題文字
    std::shared_ptr<Util::GameObject> m_titleObj;  // 標題GameObject
    std::shared_ptr<Util::Text> m_valueText;  // 數值文字
    std::shared_ptr<Util::GameObject> m_valueObj;  // 數值GameObject
    
    float m_width;
    float m_height;

public:
    StatusItem(const std::string &title, const std::string &initialValue,
               float width, float height, float zIndex = 10.0f);
    
    void updateValue(const std::string &value);
    void setSize(float width, float height);
    glm::vec2 getSize() const { return {m_width, m_height}; }
    void setPosition(const Util::PTSDPosition &position);
    void updateTextPositions();
};

// 狀態欄類，包含多個狀態項目
class StatusBar : public Util::GameObject {
private:
    std::shared_ptr<Util::Shape> m_backgroundShape;  // 背景形狀
    std::shared_ptr<StatusItem> m_moneyItem;  // 金錢狀態項
    std::shared_ptr<StatusItem> m_livesItem;  // 生命狀態項
    
    float m_width;
    float m_height;
    float m_padding;
    float m_spacing;

public:
    StatusBar(const Util::PTSDPosition &position, float width, float height,
              float zIndex = 10.0f, bool fixedSize = true);
    
    void updateMoney(int amount);
    void updateLives(int amount);
    
    void setPosition(const Util::PTSDPosition &position);
    void setSize(const glm::vec2 &size);
    glm::vec2 getSize() const { return {m_width, m_height}; }
    
    void setPadding(float padding);
    void setSpacing(float spacing);
    void updateLayout();
};

} // namespace UI

#endif // UI_STATUS_HPP