#include "UI/buttons/upgrade_btn.hpp"
#include "Util/Image.hpp"
#include "Util/Logger.hpp"
#include "Util/Text.hpp"
#include <glm/vec2.hpp>

namespace UI {

// 靜態背景圖片路徑
const std::string UpgradeButton::BACKGROUND_BUY_FOR =
    RESOURCE_DIR "/buttons/upgrades/buyFor.png";
const std::string UpgradeButton::BACKGROUND_ALREADY_BOUGHT =
    RESOURCE_DIR "/buttons/upgrades/already.png";
const std::string UpgradeButton::BACKGROUND_CANT_AFFORD =
    RESOURCE_DIR "/buttons/upgrades/canAford.png";

UpgradeButton::UpgradeButton(std::shared_ptr<Tower::Tower> tower,
                             int upgradeIndex, int &money, bool &inf,
                             const Util::PTSDPosition &pos)
    : Button("upgrade", pos, glm::vec2(68, 143), true, false),
      m_tower(tower),
      m_upgradeIndex(upgradeIndex),
      m_buttonState(ButtonState::BuyFor),
      m_basePosition(pos),
      m_moneyRef(money),
      m_lastKnownMoney(money),
      m_infinityMode(inf) {

  // 獲取升級配置
  if (upgradeIndex == 0) {
    m_config = UpgradeButtonConfigManager::GetUpgrade1Config(tower->getType());
  } else {
    m_config = UpgradeButtonConfigManager::GetUpgrade2Config(tower->getType());
  }

  // 設置 Z-index 確保在面板上方顯示
  m_ZIndex = 51.1f;

  // 初始化 UI 元件
  initializeUI();

  // 更新按鈕狀態
  updateButtonState();

  LOG_DEBUG("UPGRADE_BTN: 創建升級按鈕 - 塔類型: {}, 升級索引: {}",
            static_cast<int>(tower->getType()), upgradeIndex);
}

void UpgradeButton::initializeUI() {
  // 創建背景圖片
  m_Drawable = std::make_shared<Util::Image>(BACKGROUND_BUY_FOR);

  // 創建升級圖示
  m_iconObject = std::make_shared<Util::GameObject>(
      std::make_shared<Util::Image>(m_config.imagePath), m_ZIndex + 0.2f);
  addChild(m_iconObject);

  // 創建名稱文字物件
  updateNameTexts();

  // 創建價格文字物件
  updateCostText();

  // 計算所有元件位置
  calculateTextPositions();
}

void UpgradeButton::updateButtonState() {
  if (isUpgradeAlreadyBought()) {
    setButtonState(ButtonState::AlreadyBought);
  } else if (getCurrentMoney() >= m_config.cost || m_infinityMode) {
    setButtonState(ButtonState::BuyFor);
  } else {
    setButtonState(ButtonState::CantAfford);
  }
}

void UpgradeButton::setButtonState(ButtonState state) {
  m_buttonState = state;
  updateBackground();

  // 根據狀態設置可點擊性
  switch (state) {
  case ButtonState::BuyFor:
    setClickable(true);
    break;
  case ButtonState::AlreadyBought:
  case ButtonState::CantAfford:
    setClickable(false);
    break;
  }
}

void UpgradeButton::updateBackground() {
  if (!m_Drawable)
    return;

  std::string backgroundPath;
  switch (m_buttonState) {
  case ButtonState::BuyFor:
    backgroundPath = BACKGROUND_BUY_FOR;
    break;
  case ButtonState::AlreadyBought:
    backgroundPath = BACKGROUND_ALREADY_BOUGHT;
    break;
  case ButtonState::CantAfford:
    backgroundPath = BACKGROUND_CANT_AFFORD;
    break;
  }

  // 更新背景圖片
  m_Drawable = std::make_shared<Util::Image>(backgroundPath);
}

void UpgradeButton::updateNameTexts() {
  // 清除舊的名稱文字物件
  for (auto &nameObj : m_nameTextObjects) {
    if (nameObj) {
      RemoveChild(nameObj);
    }
  }
  m_nameTextObjects.clear();

  // 為每個名稱單詞創建文字物件
  for (size_t i = 0; i < m_config.name.size(); ++i) {
    auto nameText = std::make_shared<Util::Text>(
        RESOURCE_DIR "/font/Trebuchet MS.ttf", 18, m_config.name[i]);

    // 設置文字顏色為白色，更顯眼
    nameText->SetColor(Util::Color(255, 255, 255));

    auto nameTextObj =
        std::make_shared<Util::GameObject>(nameText, m_ZIndex + 0.3f);

    m_nameTextObjects.push_back(nameTextObj);
    addChild(nameTextObj);
  }
}

void UpgradeButton::updateCostText() {
  if (m_costTextObject) {
    RemoveChild(m_costTextObject);
  }

  auto costText =
      std::make_shared<Util::Text>(RESOURCE_DIR "/font/Trebuchet MS.ttf", 18,
                                   "$" + std::to_string(m_config.cost));

  // 設置價格文字顏色為白色，更顯眼
  costText->SetColor(Util::Color(255, 255, 255));

  m_costTextObject =
      std::make_shared<Util::GameObject>(costText, m_ZIndex + 0.3f);

  addChild(m_costTextObject);
}

void UpgradeButton::calculateTextPositions() {
  glm::vec2 basePos = m_basePosition.ToVec2();

  // 設置圖示位置（在按鈕上方中央）
  if (m_iconObject) {
    m_iconObject->m_Transform.translation = basePos + glm::vec2(-5, 50);
  }

  // 設置名稱文字位置（在按鈕中央，根據單詞數量調整）
  float textStartY = 30.0f;
  float textSpacing = 18.0f;

  for (size_t i = 0; i < m_nameTextObjects.size(); ++i) {
    if (m_nameTextObjects[i]) {
      float yOffset = textStartY - (i * textSpacing);
      m_nameTextObjects[i]->m_Transform.translation =
          basePos + glm::vec2(5, yOffset);
    }
  }

  // 設置價格文字位置（在按鈕下方）
  if (m_costTextObject) {
    float costY = -60.0f;
    m_costTextObject->m_Transform.translation = basePos + glm::vec2(5, costY);
  }
}

void UpgradeButton::refreshDisplay() {
  updateButtonState();
  calculateTextPositions();
}

void UpgradeButton::setPosition(const Util::PTSDPosition &position) {
  // 更新基礎位置
  m_basePosition = position;

  // 調用父類的 setPosition 來更新按鈕本身的位置
  Button::setPosition(position);

  // 重新計算所有子物件的位置
  calculateTextPositions();
}

void UpgradeButton::onClick() {
  if (m_buttonState != ButtonState::BuyFor) {
    return;
  }
  if (m_upgradeIndex == 0) {
    m_tower->setFirstUpgrade(m_config.cost);
  } else if (m_upgradeIndex == 1) {
    m_tower->setSecondUpgrade(m_config.cost);
  }
  refreshDisplay();
}

void UpgradeButton::pre_kill() {
  m_tower = nullptr;

  // 清除所有子物件
  for (auto &child : m_children) {
    if (child) {
      RemoveChild(child);
      child = nullptr;
    }
  }
  m_children.clear();
  m_nameTextObjects.clear();
  m_iconObject = nullptr;
  m_costTextObject = nullptr;
}

void UpgradeButton::addChild(std::shared_ptr<Util::GameObject> child) {
  if (child) {
    m_children.push_back(child);
    AddChild(child);
  }
}

bool UpgradeButton::isUpgradeAlreadyBought() const {
  return (m_tower->getInfo().firstUpgrade && m_upgradeIndex == 0) ||
         (m_tower->getInfo().secondUpgrade && m_upgradeIndex == 1);
}
} // namespace UI
