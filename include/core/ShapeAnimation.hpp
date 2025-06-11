// ShapeAnimation.hpp
#pragma once

#include "Util/Time.hpp"
#include "shape.hpp"
#include <memory>
#include <vector>

namespace Util {

class ShapeAnimation : public Core::Drawable {
public:
  enum class State { PLAY, PAUSE, ENDED, COOLDOWN };

  // 構造函數 - 接受一系列不同配置的 Shape
  ShapeAnimation(const std::vector<std::shared_ptr<Shape>> &shapes,
                 bool play = true, std::size_t interval = 100,
                 bool looping = true, std::size_t cooldown = 0);

  // 構造函數 - 接受相同類型但不同參數（顏色、大小等）的 Shape
  ShapeAnimation(ShapeType type, const std::vector<glm::vec2> &sizes,
                 const std::vector<Color> &colors, bool play = true,
                 std::size_t interval = 100, bool looping = true,
                 std::size_t cooldown = 0);

  // 設置當前幀索引
  void SetCurrentFrame(std::size_t index);

  // 播放動畫
  void Play();

  // 暫停動畫
  void Pause();

  // 獲取當前狀態
  State GetState() const { return m_State; }

  // 實現 Drawable 的 Draw 方法
  void Draw(const Core::Matrices &data) override;
  glm::vec2 GetSize() const override;

  // 更新邏輯
  void Update();

  // 獲取當前幀
  std::shared_ptr<Shape> GetCurrentFrame() const { return m_Frames[m_Index]; }

  // 設置間隔
  void SetInterval(std::size_t interval) { m_Interval = interval; }

  // 設置是否循環
  void SetLooping(bool looping) { m_Looping = looping; }

  // 設置冷卻時間
  void SetCooldown(std::size_t cooldown) { m_Cooldown = cooldown; }

private:
  std::vector<std::shared_ptr<Shape>> m_Frames;
  State m_State = State::PAUSE;
  std::size_t m_Index = 0;
  std::size_t m_Interval = 100;
  bool m_Looping = true;
  std::size_t m_Cooldown = 0;
  unsigned long m_TimeBetweenFrameUpdate = 0;
  unsigned long m_CooldownEndTime = 0;
  bool m_IsChangeFrame = false;
};

} // namespace Util
