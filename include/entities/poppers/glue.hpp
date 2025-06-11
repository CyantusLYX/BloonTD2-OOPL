#ifndef GLUE_HPP
#define GLUE_HPP
#include "Util/GameObject.hpp"
#include "components/canBuy.hpp"
#include "popper.hpp"
#include <Util/Image.hpp>
#include <Util/Position.hpp>
#include <memory>
#include <vector>

/**
 * @brief 膠水類，減慢氣球速度而不破壞它們
 *
 * 膠水是放置型的 popper，放在地圖上後靜止不動
 * 碰到氣球後讓其速度減半，但不會破壞氣球
 * 每個膠水有固定的生命值 (20)，碰到氣球後生命值減少
 */
class Glue : public popper, public CanBuy {
private:
  std::shared_ptr<Util::GameObject> m_object;
  int life = 20;           // 生命值
  bool m_draggable = true; // 可拖曳的標記
  Util::PTSDPosition m_position;

public:
  // 構造函數
  Glue(const Util::PTSDPosition &pos);

  // 擊中處理
  std::vector<bool> hit(std::vector<std::shared_ptr<Bloon>> bloons) override;

  // 獲取遊戲物件
  std::shared_ptr<Util::GameObject> get_object() override;

  // 獲取位置
  Util::PTSDPosition get_position() const override { return m_position; }

  // 設置生命值
  void setLife(int life) { this->life = life; }

  // 拖曳相關方法
  void onDragStart() override;
  void onDrag(const Util::PTSDPosition &newPosition) override;
  void onDragEnd() override;

  bool isDraggable() const override { return m_draggable; }
  void setDraggable(bool draggable) override { m_draggable = draggable; }
};
#endif
