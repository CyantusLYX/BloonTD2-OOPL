#ifndef UPGRADE_BUTTON_HPP
#define UPGRADE_BUTTON_HPP

#include "UI/button.hpp"
#include "UI/buttons/upgrade_btn_conf.hpp"
#include "Util/GameObject.hpp"
#include "entities/tower/tower.hpp"
#include "interfaces/interfaces.hpp"
#include <memory>
#include <vector>

namespace UI {

class UpgradeButton : public Button, public Interface::IUpdatable {
public:
  // 按鈕狀態枚舉
  enum class ButtonState {
    BuyFor,        // 可以購買
    AlreadyBought, // 已經購買
    CantAfford     // 買不起
  };

private:
  std::shared_ptr<Tower::Tower> m_tower; // 塔的引用
  int m_upgradeIndex;                    // 升級索引 (0 or 1)
  ButtonState m_buttonState;             // 按鈕狀態
  UpgradeButtonConfig m_config;          // 升級配置
  Util::PTSDPosition m_basePosition;     // 基礎位置
  int &m_moneyRef;                       // 指向當前金錢的引用
  int m_lastKnownMoney;                  // 上次已知的金錢快照

  // UI 元件
  std::shared_ptr<Util::GameObject> m_iconObject; // 升級圖示
  std::vector<std::shared_ptr<Util::GameObject>>
      m_nameTextObjects;                              // 名稱文字物件（多行）
  std::shared_ptr<Util::GameObject> m_costTextObject; // 價格文字物件

  // 子物件管理
  std::vector<std::shared_ptr<Util::GameObject>> m_children;

  // 背景圖片路徑
  static const std::string BACKGROUND_BUY_FOR;
  static const std::string BACKGROUND_ALREADY_BOUGHT;
  static const std::string BACKGROUND_CANT_AFFORD;

public:
  // 建構函數
  UpgradeButton(std::shared_ptr<Tower::Tower> tower, int upgradeIndex,
                int &money,
                const Util::PTSDPosition &pos = Util::PTSDPosition(0, 0));

  // 解構函數
  ~UpgradeButton() override = default;

  // 更新按鈕狀態
  void updateButtonState();

  // 刷新顯示內容
  void refreshDisplay();

  // 設置按鈕狀態
  void setButtonState(ButtonState state);

  // 獲取按鈕狀態
  ButtonState getButtonState() const { return m_buttonState; }

  // 獲取升級配置
  const UpgradeButtonConfig &getConfig() const { return m_config; }

  // 獲取升級索引
  int getUpgradeIndex() const { return m_upgradeIndex; }

  // 點擊事件處理
  void onClick() override;

  // 設置位置並更新所有子物件位置
  void setPosition(const Util::PTSDPosition &position) override;

  // 生命週期管理
  void pre_kill() override;

  int getCost() const { return m_config.cost; } // 獲取升級價格

private:
  // 初始化 UI 元件
  void initializeUI();

  // 更新背景圖片
  void updateBackground();

  // 更新名稱文字
  void updateNameTexts();

  // 更新價格文字
  void updateCostText();

  // 計算文字位置
  void calculateTextPositions();

  // 更新顯示（檢查金錢變化）
  void update() override {
    if (m_moneyRef != m_lastKnownMoney) {
      m_lastKnownMoney = m_moneyRef;
      refreshDisplay(); // 更新顯示
    }
  }

  // 添加子物件
  void addChild(std::shared_ptr<Util::GameObject> child);

  // 獲取當前金錢數量
  int getCurrentMoney() const { return m_moneyRef; }

  // 檢查是否已經購買該升級
  bool isUpgradeAlreadyBought() const;
};

} // namespace UI

#endif // UPGRADE_BUTTON_HPP
