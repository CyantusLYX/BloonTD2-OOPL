#include "UI/Flag.hpp"
#include "UI/button.hpp"
#include "UI/buttons/upgrade_btn.hpp"
#include "Util/GameObject.hpp"
#include "components/mortal.hpp"
#include "entities/tower/tower.hpp"
#include "interfaces/interfaces.hpp"
#include <memory>
#include <vector>
namespace UI {
class SellButton : public Button {
private:
  std::shared_ptr<Tower::Tower> m_tower; // 當前塔的引用
  int m_sellPrice;                       // 賣出價格
public:
  SellButton(std::shared_ptr<Tower::Tower> tower,
             const Util::PTSDPosition &pos);
  ~SellButton() = default;
  void refreshSellPrice();                         // 刷新賣出價格
  void onClick() override {};                      // 點擊事件處理
  int getSellPrice() const { return m_sellPrice; } // 獲取賣出價格
  std::shared_ptr<Tower::Tower> getTower() const {
    return m_tower;
  } // 獲取塔資訊
};
class UpgradesPanel : public Flag, public Interface::IUpdatable {
private:
  std::shared_ptr<Util::GameObject> m_background;      // 背景 GameObject
  std::shared_ptr<Tower::Tower> m_tower;               // 當前塔的引用
  std::shared_ptr<Util::GameObject> m_nameTextObject;  // 名稱文本 GameObject
  std::shared_ptr<Util::GameObject> m_speedTextObject; // 速度文本 GameObject
  std::shared_ptr<Util::GameObject> m_rangeTextObject; // 範圍文本 GameObject
  std::shared_ptr<UpgradeButton> m_upgradeBtn1Object;  // 升級按鈕1
  std::shared_ptr<UpgradeButton> m_upgradeBtn2Object;  // 升級按鈕2
  std::shared_ptr<SellButton> m_sellBtnObject;         // 賣出按鈕 GameObject
  std::vector<std::shared_ptr<Util::GameObject>> children_ct;
  int &m_money;         // 引用當前金錢
  bool &m_infinityMode; // 引用無限模式開關
public:
  void update() override {
    m_sellBtnObject->refreshSellPrice(); // 刷新賣出價格
    refreshAllTexts();                   // 刷新所有文字
  }

  // 刷新所有文字內容
  void refreshAllTexts();

  void pre_kill() override {
    m_tower = nullptr; // 清除塔的引用
    if (m_sellBtnObject)
      m_sellBtnObject->kill();
    if (m_upgradeBtn1Object)
      m_upgradeBtn1Object->kill();
    if (m_upgradeBtn2Object)
      m_upgradeBtn2Object->kill();
    for (auto &child : children_ct) {
      RemoveChild(child); // 移除所有子物件
      child = nullptr;    // 清除子物件引用
    }
  }
  std::shared_ptr<SellButton> getSellButton() { return m_sellBtnObject; }
  std::vector<std::shared_ptr<Button>> getButtons() {
    return {m_upgradeBtn1Object, m_upgradeBtn2Object, m_sellBtnObject};
  }
  std::shared_ptr<UpgradeButton> getUpgradeButton1() {
    return m_upgradeBtn1Object;
  }
  std::shared_ptr<UpgradeButton> getUpgradeButton2() {
    return m_upgradeBtn2Object;
  }
  UpgradesPanel(std::shared_ptr<Tower::Tower> tower, int &money, bool &inf);
  ~UpgradesPanel() override = default;
};
}; // namespace UI
