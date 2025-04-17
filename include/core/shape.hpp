#pragma once

#include "Core/Drawable.hpp"
#include "Core/Program.hpp"
#include "Core/UniformBuffer.hpp"
#include "Core/VertexArray.hpp"
#include "Util/Color.hpp"
#include <memory>
#include <vector>

namespace Util {

// 定義可用的形狀類型
enum class ShapeType { Rectangle, Circle, Triangle, Ellipse, Line };

class Shape : public Core::Drawable {
public:
  // 建構函數
  Shape(ShapeType type, const glm::vec2 &size,
        const Util::Color &color = Color::FromName(Colors::WHITE));
  virtual ~Shape() = default;

  // 顏色設置方法 - 提供多種便捷方式設置顏色
  void SetColor(const Color &color); // 直接設置 Color 對象

  // 便捷設置顏色的方法
  void SetColorRGB(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
  void SetColorHSV(float h, float s, float v, float a = 1.0f);
  void SetColorHSL(float h, float s, float l, float a = 1.0f);
  void SetColorHex(Uint32 hex);
  void SetColorHex(const std::string &hex);
  void SetColorName(const Colors &name);

  // 設置透明度 (會更新 Color 的 alpha 值)
  void SetAlpha(float alpha);

  // 其他設置
  void SetSize(const glm::vec2 &size);
  void SetOutline(bool outline, float width = 1.0f);

  // 取得形狀屬性
  ShapeType GetType() const;
  Color GetColor() const;
  float GetAlpha() const;
  bool HasOutline() const;
  float GetOutlineWidth() const;

  // 實現 Drawable 介面
  void Draw(const Core::Matrices &data) override;
  glm::vec2 GetSize() const override;

private:
  ShapeType m_Type;
  glm::vec2 m_Size;
  Color m_Color; // 包含 RGBA
  bool m_HasOutline;
  float m_OutlineWidth;

  // 著色器程序和頂點緩衝區
  std::unique_ptr<Core::UniformBuffer<Core::Matrices>> m_UniformBuffer;

  // 靜態資源 - 所有形狀共享
  static std::unique_ptr<Core::Program> s_Program;
  static std::unique_ptr<Core::VertexArray> s_RectangleVA;
  static std::unique_ptr<Core::VertexArray> s_CircleVA;
  static std::unique_ptr<Core::VertexArray> s_TriangleVA;
  static std::unique_ptr<Core::VertexArray> s_EllipseVA;
  static std::unique_ptr<Core::VertexArray> s_LineVA;

  // 初始化靜態資源
  static void InitializeShaders();
  static void InitializeVertexArrays();
  static void InitializeRectangle();
  static void InitializeCircle(int segments = 32);
  static void InitializeTriangle();
  static void InitializeEllipse(int segments = 32);
  static void InitializeLine();
};

} // namespace Util