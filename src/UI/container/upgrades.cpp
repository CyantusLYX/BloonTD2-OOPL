#include "UI/container/upgrades.hpp"
#include "Util/Image.hpp"
#include "Util/Text.hpp"
#include "conf.hpp"
#include "interfaces/interfaces.hpp"
#include <glm/fwd.hpp>
#include <magic_enum/magic_enum.hpp>
UI::UpgradesPanel::UpgradesPanel(std::shared_ptr<Tower::Tower> tower,
                                 int &money, bool &inf)
    : Flag(),
      m_tower(tower),
      m_money(money),
      m_infinityMode(inf) {
  // 創建背景 GameObject
  m_background = std::make_shared<Util::GameObject>(
      std::make_shared<Util::Image>(RESOURCE_DIR "/UI/UpgradeBase.png"), 50);

  // 設置位置
  setPosition(Util::PTSDPosition(235, -30));

  // 創建文本 GameObject（空的，稍後會填入內容）
  m_nameTextObject = std::make_shared<Util::GameObject>(
      std::make_shared<Util::Text>(
          RESOURCE_DIR "/font/NotoSansTC-ExtraLight.ttf", 12, "d"),
      50.1);

  m_speedTextObject = std::make_shared<Util::GameObject>(
      std::make_shared<Util::Text>(
          RESOURCE_DIR "/font/NotoSansTC-ExtraLight.ttf", 12, "d"),
      50.1);

  m_rangeTextObject = std::make_shared<Util::GameObject>(
      std::make_shared<Util::Text>(
          RESOURCE_DIR "/font/NotoSansTC-ExtraLight.ttf", 12, "d"),
      50.1);

  // 創建升級按鈕1 GameObject
  m_upgradeBtn1Object = std::make_shared<UI::UpgradeButton>(
      m_tower, 0, m_money, m_infinityMode, Util::PTSDPosition(0, 0));

  // 創建升級按鈕2 GameObject
  m_upgradeBtn2Object = std::make_shared<UI::UpgradeButton>(
      m_tower, 1, m_money, m_infinityMode, Util::PTSDPosition(0, 0));

  // 創建賣出按鈕 GameObject
  m_sellBtnObject =
      std::make_shared<UI::SellButton>(m_tower, Util::PTSDPosition(0, 0));

  m_nameTextObject = std::make_shared<Util::GameObject>(
      std::make_shared<Util::Text>(RESOURCE_DIR
                                   "/font/NotoSansTC-ExtraLight.ttf",
                                   12, m_tower->getInfo().name),
      50.1);

  // 設置位置
  m_background->m_Transform.translation = (basePosition).ToVec2();
  m_nameTextObject->m_Transform.translation =
      (basePosition + glm::vec2(10, 110)).ToVec2();
  m_speedTextObject->m_Transform.translation =
      (basePosition + glm::vec2(40, 90)).ToVec2();
  m_rangeTextObject->m_Transform.translation =
      (basePosition + glm::vec2(40, 70)).ToVec2();
  m_upgradeBtn1Object->setPosition(basePosition +
                                   glm::vec2(-35, -15)); // 升級按鈕1位置
  m_upgradeBtn2Object->setPosition(basePosition +
                                   glm::vec2(35, -15)); // 升級按鈕2位置
  m_sellBtnObject->setPosition(basePosition +
                               glm::vec2(0, -100)); // 賣出按鈕位置
  m_sellBtnObject->m_Transform.translation =
      (basePosition + glm::vec2(60, -104)).ToVec2();

  children_ct.push_back(m_background);
  children_ct.push_back(m_nameTextObject);
  children_ct.push_back(m_speedTextObject);
  children_ct.push_back(m_rangeTextObject);
  children_ct.push_back(m_upgradeBtn1Object);
  children_ct.push_back(m_upgradeBtn2Object);
  children_ct.push_back(m_sellBtnObject);
  for (const auto &child : children_ct) {
    AddChild(child);
  }

  // 初始化所有文字內容
  refreshAllTexts();
  Interface::IUpdatable::add_child(m_upgradeBtn1Object);
  Interface::IUpdatable::add_child(m_upgradeBtn2Object);
};
UI::SellButton::SellButton(std::shared_ptr<Tower::Tower> tower,
                           const Util::PTSDPosition &pos)
    : Button("sell", pos, glm::vec2(138, 25), true, false),
      m_tower(tower) {
  refreshSellPrice();
  setClickable(true);
  m_ZIndex = 50.1f; // 確保賣出按鈕在升級面板上方
}
void UI::SellButton::refreshSellPrice() {
  m_sellPrice = static_cast<int>(m_tower->getInfo().investmentCost * SELL_RATE);
  auto new_text = std::make_shared<Util::Text>(
      RESOURCE_DIR "/font/NotoSansTC-ExtraLight.ttf", 12,
      std::to_string(m_sellPrice));
  new_text->SetColor(Util::Color(0, 0, 0)); // 設置文字顏色為黑色
  m_Drawable = new_text;
}

void UI::UpgradesPanel::refreshAllTexts() {
  // 更新名稱文本
  auto nameText = std::make_shared<Util::Text>(
      RESOURCE_DIR "/font/NotoSansTC-ExtraLight.ttf", 12,
      m_tower->getInfo().name);
  m_nameTextObject->SetDrawable(nameText);

  // 更新速度文本
  auto speedText = std::make_shared<Util::Text>(
      RESOURCE_DIR "/font/NotoSansTC-ExtraLight.ttf", 12,
      std::string(magic_enum::enum_name(m_tower->getInfo().attackSpeed)));
  m_speedTextObject->SetDrawable(speedText);

  // 更新範圍文本
  auto rangeText = std::make_shared<Util::Text>(
      RESOURCE_DIR "/font/NotoSansTC-ExtraLight.ttf", 12,
      std::to_string(static_cast<int>(m_tower->getInfo().attackRange)));
  m_rangeTextObject->SetDrawable(rangeText);

  this->m_sellBtnObject->refreshSellPrice();
}
