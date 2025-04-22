#ifndef UI_CONTAINER_HPP
#define UI_CONTAINER_HPP

#include "Util/GameObject.hpp"
#include "Util/Position.hpp"
#include "components/collisionComp.hpp"
#include "interfaces/clickable.hpp"
#include <memory>
#include <vector>
#include <glm/glm.hpp>

namespace UI {

class UIContainer : 
    public Util::GameObject, 
    public Components::CollisionComponent, 
    public Interface::I_clickable,
    public std::enable_shared_from_this<UIContainer>  {
private:
    // 互動狀態
    bool m_clickable = true;
    
    // 佈局參數
    float m_padding = 5.0f;  // 內邊距
    float m_spacing = 2.0f;  // 子元素間距
    
public:
    // 建構函式
    UIContainer(
        const Util::PTSDPosition &position, 
        const glm::vec2 &size,
        float zIndex = 10.0f,
        bool visible = true
    );
    virtual ~UIContainer() = default;
    
    // I_clickable 介面實現
    bool isClickable() const override { return m_clickable; }
    void setClickable(bool clickable) override { m_clickable = clickable; }
    virtual void onClick() override;  // 默認為空，由子類實現
    virtual void onFocus() override {}  // 默認為空，由子類實現
    
    // 增強的子節點管理
    void addChild(const std::shared_ptr<Util::GameObject> &child, bool updateLayout = true);
    void removeChild(const std::shared_ptr<Util::GameObject> &child, bool updateLayout = true);
    void clearChildren();
    
    // 計算子節點的絕對位置（考慮父節點變換）
    Util::PTSDPosition getWorldPosition(const std::shared_ptr<Util::GameObject> &child) const;
    
    // 將世界座標轉換為容器內的局部座標
    Util::PTSDPosition toLocalPosition(const Util::PTSDPosition &worldPos) const;
    
    // 獲取/設置佈局參數
    void setPadding(float padding) { m_padding = padding; }
    float getPadding() const { return m_padding; }
    
    void setSpacing(float spacing) { m_spacing = spacing; }
    float getSpacing() const { return m_spacing; }
    
    // 根據子節點自動調整容器大小
    virtual void resizeToFitChildren();
    
    // 更新子節點佈局
    virtual void updateLayout();
    
    // 設置位置，同時更新碰撞組件
    void setPosition(const Util::PTSDPosition &position) override;
    
    // 設置大小，同時更新碰撞組件和形狀
    void setSize(const glm::vec2 &size);

    // 檢查點是否在容器內（考慮子元素）
    bool containsPoint(const Util::PTSDPosition &point) const;

    // 查找點擊的子元素
    std::shared_ptr<Util::GameObject> findChildAt(const Util::PTSDPosition &point) const;
};

}  // namespace UI

#endif // UI_CONTAINER_HPP