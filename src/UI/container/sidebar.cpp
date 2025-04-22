#include "UI/container/sidebar.hpp"
#include "Util/Logger.hpp"
#include "core/shape.hpp"
#include "Util/GameObject.hpp"
#include "Util/Text.hpp"

namespace UI {

UISidebar::UISidebar(
    const Util::PTSDPosition &position,
    float height,
    float width,
    float zIndex
) : UIContainer(position, glm::vec2(width, height), zIndex) {
    
    // 設置側邊欄外觀
    auto shape = std::dynamic_pointer_cast<Util::Shape>(m_Drawable);
    if (shape) {
        shape->SetColorRGB(180, 180, 180, 220); // 半透明灰色背景
    }
    
    // 設置較大的內邊距，美觀用
    setPadding(10.0f);
    setSpacing(5.0f);
    
    // 計算每個區域的高度比例
    float statusHeight = height * 0.12f;  // 12% 給狀態區
    float buyHeight = height * 0.38f;     // 38% 給購買區
    float infoHeight = height * 0.35f;    // 35% 給塔資訊區
    float controlHeight = height * 0.15f; // 15% 給控制區
    
    // 創建狀態區域容器 (頂部)
    m_statusSection = std::make_shared<UIContainer>(
        Util::PTSDPosition(0, 0),  // 位置將在更新佈局時設置
        glm::vec2(width - 2*getPadding(), statusHeight),
        zIndex + 0.1f
    );
    m_statusSection->setPadding(5.0f);
    
    // 創建狀態文字 (注意：文字需要包裝在 GameObject 中)
    // 1. 首先創建 Text 對象
    auto moneyTextDrawable = std::make_shared<Util::Text>(
        "fonts/DroidSans.ttf", 18, "Money: 0", Util::Color(255, 255, 255));
    auto livesTextDrawable = std::make_shared<Util::Text>(
        "fonts/DroidSans.ttf", 18, "Lives: 0", Util::Color(255, 255, 255));
    
    // 2. 然後將 Text 包裝在 GameObject 中
    m_moneyTextObj = std::make_shared<Util::GameObject>(
        moneyTextDrawable, zIndex + 0.2f);
    m_livesTextObj = std::make_shared<Util::GameObject>(
        livesTextDrawable, zIndex + 0.2f);
    
    // 3. 將 GameObject 添加為子元素
    m_statusSection->addChild(m_moneyTextObj);
    m_statusSection->addChild(m_livesTextObj);
    
    // 4. 保存 Text 對象的引用以便於更新文字內容
    m_moneyText = moneyTextDrawable;
    m_livesText = livesTextDrawable;
    
    // 波數不在狀態區，可能在其他地方顯示
    
    // 創建購買區域容器
    m_buySection = std::make_shared<UIContainer>(
        Util::PTSDPosition(0, 0),  // 位置將在更新佈局時設置
        glm::vec2(width - 2*getPadding(), buyHeight),
        zIndex + 0.1f
    );
    m_buySection->setPadding(5.0f);
    
    // 添加"Buy Stuff"標題
    auto buyTitleTextDrawable = std::make_shared<Util::Text>(
        "fonts/DroidSans.ttf", 20, "Buy Stuff", Util::Color(255, 255, 255));
    auto buyTitleObj = std::make_shared<Util::GameObject>(
        buyTitleTextDrawable, zIndex + 0.2f);
    m_buySection->addChild(buyTitleObj);
    
    // 創建塔資訊區域容器
    m_towerInfoSection = std::make_shared<UIContainer>(
        Util::PTSDPosition(0, 0),  // 位置將在更新佈局時設置
        glm::vec2(width - 2*getPadding(), infoHeight),
        zIndex + 0.1f
    );
    m_towerInfoSection->setPadding(5.0f);
    m_towerInfoSection->SetVisible(false); // 預設隱藏
    
    // 創建控制區域容器
    m_controlSection = std::make_shared<UIContainer>(
        Util::PTSDPosition(0, 0),  // 位置將在更新佈局時設置
        glm::vec2(width - 2*getPadding(), controlHeight),
        zIndex + 0.1f
    );
    m_controlSection->setPadding(5.0f);
    
    // 添加所有區域容器到側邊欄
    addChild(m_statusSection);
    addChild(m_buySection);
    addChild(m_towerInfoSection);
    addChild(m_controlSection);
    
    // 更新佈局
    updateLayout();
    
    LOG_DEBUG("UISidebar: Created with dimensions {}x{}", width, height);
}

void UISidebar::updateMoney(int amount) {
    if (m_moneyText) {
        m_moneyText->SetText("Money: " + std::to_string(amount));
    }
}

void UISidebar::updateLives(int amount) {
    if (m_livesText) {
        m_livesText->SetText("Lives: " + std::to_string(amount));
    }
}

void UISidebar::clearSections() {
    if (m_statusSection) m_statusSection->clearChildren();
    if (m_buySection) m_buySection->clearChildren();
    if (m_towerInfoSection) m_towerInfoSection->clearChildren();
    if (m_controlSection) m_controlSection->clearChildren();
    
    // 重新添加基本的狀態文字
    if (m_statusSection && m_moneyTextObj && m_livesTextObj) {
        m_statusSection->addChild(m_moneyTextObj);
        m_statusSection->addChild(m_livesTextObj);
    }
}

// 其餘方法保持不變...

} // namespace UI