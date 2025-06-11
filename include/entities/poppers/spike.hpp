#ifndef SPIKE_HPP
#define SPIKE_HPP
#include "Util/GameObject.hpp"
#include "components/canBuy.hpp"
#include "popper.hpp"
#include <Util/Image.hpp>
#include <Util/Position.hpp>
#include <memory>
#include <vector>
class spike : public popper, public CanBuy {
private:
  std::shared_ptr<Util::GameObject> m_object;
  int life = 10;
  bool m_draggable = false; // 新增: 是否可拖曳的標記

public:
  spike(const Util::PTSDPosition &pos);
  std::vector<bool> hit(std::vector<std::shared_ptr<Bloon>> bloons) override;
  std::shared_ptr<Util::GameObject> get_object() override;
  Util::PTSDPosition get_position() const override {
    auto pos = m_object->m_Transform.translation;
    return Util::PTSDPosition(pos.x, pos.y);
  }
  void setLife(int life) { this->life = life; }
  virtual void onDragStart() override;
  virtual void onDrag(const Util::PTSDPosition &newPosition) override;
  virtual void onDragEnd() override;

  virtual bool isDraggable() const override { return m_draggable; }
  virtual void setDraggable(bool draggable) override {
    m_draggable = draggable;
  }
};
#endif
