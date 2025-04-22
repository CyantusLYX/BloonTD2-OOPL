#ifndef UI_SIDEBAR_HPP
#define UI_SIDEBAR_HPP

#include "UI/container/container.hpp"
#include "Util/Text.hpp"
#include <memory>
#include <string>

namespace UI {

// 側邊欄的主類別
class UISidebar : public UIContainer {
private:
    // 保存各個功能區域的子容器
    std::shared_ptr<UIContainer> m_statusSection;  // 狀態區（金錢、生命等）
    std::shared_ptr<UIContainer> m_buySection;     // 購買塔區域
    std::shared_ptr<UIContainer> m_towerInfoSection; // 塔資訊/升級區域
    std::shared_ptr<UIContainer> m_controlSection;  // 控制按鈕區域（開始按鈕等）
    
    // 狀態文字對象與其GameObject容器
    std::shared_ptr<Util::GameObject> m_moneyTextObj;  // GameObject 容器
    std::shared_ptr<Util::GameObject> m_livesTextObj;  // GameObject 容器
    std::shared_ptr<Util::GameObject> m_waveTextObj;  // GameObject 容器
    
    // Text 對象引用
    std::shared_ptr<Util::Text> m_moneyText;
    std::shared_ptr<Util::Text> m_livesText;
    std::shared_ptr<Util::Text> m_waveText;
    
public:
    // 建構函式
    UISidebar(
        const Util::PTSDPosition &position, 
        float height,
        float width = 200.0f,
        float zIndex = 10.0f
    );
    
    // 狀態更新函式
    void updateMoney(int amount);
    void updateLives(int amount);
    void updateWave(int wave);
    
    // 獲取各個區域，方便外部添加元素
    std::shared_ptr<UIContainer> getStatusSection() const { return m_statusSection; }
    std::shared_ptr<UIContainer> getBuySection() const { return m_buySection; }
    std::shared_ptr<UIContainer> getTowerInfoSection() const { return m_towerInfoSection; }
    std::shared_ptr<UIContainer> getControlSection() const { return m_controlSection; }
    
    // 清除所有區域的內容
    void clearSections();
    
    // 更新佈局 (覆蓋基類方法)
    void updateLayout() override;
};

} // namespace UI

#endif // UI_SIDEBAR_HPP