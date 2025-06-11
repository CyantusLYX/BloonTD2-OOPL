#ifndef UI_FLAG_HPP
#define UI_FLAG_HPP

#include "Util/GameObject.hpp"
#include "Util/Position.hpp"
#include "components/mortal.hpp"

namespace UI {

// Flag 基類 - 用於顯示Tower資訊或升級選項
class Flag : public Util::GameObject, public Mortal {
private:
protected:
  Util::PTSDPosition basePosition; // 基礎位置
  glm::vec2 m_size = {148.0f, 243.0f}; // 預設大小

  // Protected constructor: 只有衍生類別可以呼叫
  // 它需要一個初始位置來設定 basePosition
  explicit Flag(
      const Util::PTSDPosition &initialPosition = Util::PTSDPosition(0, 0))
      : Util::GameObject(),
        basePosition(initialPosition) {
    // 設定 GameObject 的位置
    // 假設 m_Transform 是從 Util::GameObject 繼承來的，並且是可訪問的
    // 或者 Util::GameObject 有一個 SetPosition 之類的方法
    m_Transform.translation = initialPosition.ToVec2();
  }

  virtual ~Flag() = default; // 基底類別的解構函式應為 virtual
public:
  // 設置位置
  void setPosition(const Util::PTSDPosition &position) {
    m_Transform.translation = position.ToVec2(); // 更新 GameObject 的位置
    basePosition = position;                     // 更新 Flag 特有的基礎位置
  }

  // 獲取當前位置
  Util::PTSDPosition getPosition() const { return basePosition; }
  // 獲取大小
  glm::vec2 getSize() const { return m_size; }
};
} // namespace UI

#endif // UI_FLAG_HPP