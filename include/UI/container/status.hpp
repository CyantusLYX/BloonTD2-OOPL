#ifndef UI_STATUS_HPP
#define UI_STATUS_HPP

#include "UI/container/container.hpp"
#include "Util/GameObject.hpp"
#include "Util/Text.hpp"
#include <memory>
#include <string>

namespace UI {

// 單一狀態項目類，用於顯示一個標題和它對應的數值
class StatusItem : public UIContainer {
private:
  std::shared_ptr<UIContainer> m_titleContainer; // 標題容器
  std::shared_ptr<UIContainer> m_valueContainer; // 數值容器

  std::shared_ptr<Util::GameObject> m_titleObj; // 標題的GameObject
  std::shared_ptr<Util::GameObject> m_valueObj; // 數值的GameObject

  std::shared_ptr<Util::Text> m_titleText; // 標題文字引用
  std::shared_ptr<Util::Text> m_valueText; // 數值文字引用

public:
  StatusItem(const std::string &title, const std::string &initialValue,
             float width, float height, float zIndex = 10.0f);

  void updateValue(const std::string &value);
};

// 狀態欄類，包含多個狀態項目
class StatusBar : public UIContainer {
private:
  std::shared_ptr<StatusItem> m_moneyItem; // 金錢狀態項
  std::shared_ptr<StatusItem> m_livesItem; // 生命狀態項

public:
  StatusBar(const Util::PTSDPosition &position, float width, float height,
            float zIndex = 10.0f);

  void updateMoney(int amount);
  void updateLives(int amount);
};

} // namespace UI

#endif // UI_STATUS_HPP