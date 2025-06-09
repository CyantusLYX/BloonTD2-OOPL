#include "UI/container/upgrades.hpp"
#include "Util/Image.hpp"
#include "Util/Text.hpp"
#include "conf.hpp"
#include <glm/fwd.hpp>
#include <magic_enum/magic_enum.hpp>
UI::UpgradesPanel::UpgradesPanel(std::shared_ptr<Tower::Tower> tower)
    : Flag(), m_tower(tower) {
  // 創建背景 GameObject
  m_background = std::make_shared<Util::GameObject>(
      std::make_shared<Util::Image>(RESOURCE_DIR "/UI/UpgradeBase.png"), 50);

  // 設置位置
  setPosition(Util::PTSDPosition(235, -30));

  // 創建名稱文本 GameObject
  m_nameTextObject = std::make_shared<Util::GameObject>(
      std::make_shared<Util::Text>(RESOURCE_DIR
                                   "/font/NotoSansTC-ExtraLight.ttf",
                                   12, tower->getInfo().name),
      50.1); // Z-index

  // 創建速度文本 GameObject
  m_speedTextObject = std::make_shared<Util::GameObject>(
      std::make_shared<Util::Text>(
          RESOURCE_DIR "/font/NotoSansTC-ExtraLight.ttf", 12,
          std::string(magic_enum::enum_name(tower->getInfo().attackSpeed))),
      50.1); // Z-index

  // 創建範圍文本 GameObject
  m_rangeTextObject = std::make_shared<Util::GameObject>(
      std::make_shared<Util::Text>(
          RESOURCE_DIR "/font/NotoSansTC-ExtraLight.ttf", 12,
          std::to_string(static_cast<int>(tower->getInfo().attackRange))),
      50.1); // Z-index

  // 創建賣出按鈕 GameObject
  m_sellBtnObject =
      std::make_shared<UI::SellButton>(m_tower, Util::PTSDPosition(0, 0));

  // 設置位置
  m_background->m_Transform.translation = (basePosition).ToVec2();
  m_nameTextObject->m_Transform.translation =
      (basePosition + glm::vec2(10, 110)).ToVec2();
  m_speedTextObject->m_Transform.translation =
      (basePosition + glm::vec2(40, 90)).ToVec2();
  m_rangeTextObject->m_Transform.translation =
      (basePosition + glm::vec2(40, 70)).ToVec2();
  m_sellBtnObject->setPosition(basePosition + glm::vec2(10, -100)); // 賣出按鈕位置

  children_ct.push_back(m_background);
  children_ct.push_back(m_nameTextObject);
  children_ct.push_back(m_speedTextObject);
  children_ct.push_back(m_rangeTextObject);
  children_ct.push_back(m_sellBtnObject);
  for (const auto &child : children_ct) {
    AddChild(child);
  }
};
UI::SellButton::SellButton(std::shared_ptr<Tower::Tower> tower,
                           const Util::PTSDPosition &pos)
    : Button("sell", pos, glm::vec2(138, 25), true, false), m_tower(tower) {
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
