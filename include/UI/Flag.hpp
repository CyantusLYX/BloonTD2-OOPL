#ifndef UI_FLAG_HPP
#define UI_FLAG_HPP

#include "Util/GameObject.hpp"
#include "Util/Position.hpp"
#include "core/shape.hpp"
#include <memory>
#include <string>

namespace UI {

// Flag 基類 - 用於顯示Tower資訊或升級選項
class Flag : public Util::GameObject {
protected:
    // 直接存儲背景形狀以便於訪問
    std::shared_ptr<Util::Shape> m_backgroundShape;
    
    // 尺寸
    glm::vec2 m_size;
    
    // 內部邊距和間距
    float m_padding;
    float m_spacing;

public:
    Flag(const Util::PTSDPosition &position, const glm::vec2 &size, float zIndex = 10.0f);
    virtual ~Flag() = default;
    
    // 設置標題 - 子類實現
    virtual void setTitle(const std::string& title) = 0;
    
    // 清除所有內容 - 子類實現
    virtual void clear() = 0;
    
    // 設置尺寸
    virtual void setSize(const glm::vec2 &size){};
    glm::vec2 getSize() const { return m_size; }
    
    // 佈局控制
    void setPadding(float padding) { m_padding = padding; }
    void setSpacing(float spacing) { m_spacing = spacing; }
    void setPosition(const Util::PTSDPosition &position) { m_Transform.translation = position.ToVec2(); }
    
    // 獲取背景形狀
    std::shared_ptr<Util::Shape> getBackgroundShape() const { return m_backgroundShape; }
};

// 未來將實現的子類 (暫不實現)
// class InfoFlag : public Flag { ... };
// class UpgradeFlag : public Flag { ... };

} // namespace UI

#endif // UI_FLAG_HPP