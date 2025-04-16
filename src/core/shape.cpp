#include "core/shape.hpp"
#include "Core/IndexBuffer.hpp"
#include "Core/VertexBuffer.hpp"
#include "Util/Logger.hpp"
#include "config.hpp"
#include <algorithm>
#include <cmath>

namespace Util {

// 靜態成員初始化
std::unique_ptr<Core::Program> Shape::s_Program = nullptr;
std::unique_ptr<Core::VertexArray> Shape::s_RectangleVA = nullptr;
std::unique_ptr<Core::VertexArray> Shape::s_CircleVA = nullptr;
std::unique_ptr<Core::VertexArray> Shape::s_TriangleVA = nullptr;
std::unique_ptr<Core::VertexArray> Shape::s_EllipseVA = nullptr;
std::unique_ptr<Core::VertexArray> Shape::s_LineVA = nullptr;

Shape::Shape(ShapeType type, const glm::vec2 &size, const Color &color)
    : m_Type(type), m_Size(size), m_Color(color), m_HasOutline(false),
      m_OutlineWidth(1.0f) {
  // 初始化著色器程序（如果尚未初始化）
  if (s_Program == nullptr) {
    InitializeShaders();
  }

  // 初始化頂點陣列（如果尚未初始化）
  if (s_RectangleVA == nullptr) {
    InitializeVertexArrays();
  }

  // 創建 Uniform Buffer
  m_UniformBuffer = std::make_unique<Core::UniformBuffer<Core::Matrices>>(
      *s_Program, "Matrices", 0);
}

void Shape::SetColor(const Color &color) { m_Color = color; }

void Shape::SetColorRGB(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
  m_Color = Color::FromRGB(r, g, b, a);
}

void Shape::SetColorHSV(float h, float s, float v, float a) {
  m_Color = Color::FromHSV(h, s, v, a);
}

void Shape::SetColorHex(Uint32 hex) { m_Color = Color::FromHex(hex); }

void Shape::SetColorHex(const std::string &hex) {
  m_Color = Color::FromHex(hex);
}

void Shape::SetColorHSL(float h, float s, float l, float a) {
  m_Color = Color::FromHSL(h, s, l, a);
}

void Shape::SetColorName(const Colors &name) {
  m_Color = Color::FromName(name);
}

void Shape::SetAlpha(float alpha) {
  float clampedAlpha = std::clamp(alpha, 0.0f, 1.0f);
  m_Color.a = clampedAlpha * 255.0f; // 將 0-1 範圍的值轉為 0-255
}

void Shape::SetSize(const glm::vec2 &size) { m_Size = size; }

void Shape::SetOutline(bool outline, float width) {
  m_HasOutline = outline;
  m_OutlineWidth = std::max(0.1f, width);
}

ShapeType Shape::GetType() const { return m_Type; }

Color Shape::GetColor() const { return m_Color; }

float Shape::GetAlpha() const { return m_Color.a / 255.0f; }

bool Shape::HasOutline() const { return m_HasOutline; }

float Shape::GetOutlineWidth() const { return m_OutlineWidth; }

glm::vec2 Shape::GetSize() const { return m_Size; }

void Shape::Draw(const Core::Matrices &data) {
  // 設置 uniform 數據
  m_UniformBuffer->SetData(0, data);

  // 綁定著色器
  s_Program->Bind();

  // 設置顏色和透明度 uniform - 使用 Color 的原始值
  GLint colorLoc = glGetUniformLocation(s_Program->GetId(), "shapeColor");
  glUniform4f(colorLoc,
              m_Color.r / 255.0f, // 將 Color 的 0-255 範圍轉換為 GL 的 0-1 範圍
              m_Color.g / 255.0f, m_Color.b / 255.0f, m_Color.a / 255.0f);

  // 設置是否有邊框的 uniform
  GLint outlineLoc = glGetUniformLocation(s_Program->GetId(), "hasOutline");
  glUniform1i(outlineLoc, m_HasOutline ? 1 : 0);

  // 設置邊框寬度的 uniform
  GLint outlineWidthLoc =
      glGetUniformLocation(s_Program->GetId(), "outlineWidth");
  glUniform1f(outlineWidthLoc, m_OutlineWidth);

  // 根據形狀類型選擇合適的頂點陣列
  Core::VertexArray *va = nullptr;
  switch (m_Type) {
  case ShapeType::Rectangle:
    va = s_RectangleVA.get();
    break;
  case ShapeType::Circle:
    va = s_CircleVA.get();
    break;
  case ShapeType::Triangle:
    va = s_TriangleVA.get();
    break;
  case ShapeType::Ellipse:
    va = s_EllipseVA.get();
    break;
  case ShapeType::Line:
    va = s_LineVA.get();
    break;
  default:
    LOG_ERROR("Unknown shape type");
    return;
  }

  // 繪製形狀
  if (va) {
    va->Bind();
    va->DrawTriangles();
  }
}

void Shape::InitializeShaders() {
  // 創建著色器程序
  s_Program = std::make_unique<Core::Program>(
      RESOURCE_DIR "/shaders/shape.vert", RESOURCE_DIR "/shaders/shape.frag");

  if (!s_Program) {
    LOG_ERROR("Failed to create shape shader program");
  }
}

void Shape::InitializeVertexArrays() {
  InitializeRectangle();
  InitializeCircle();
  InitializeTriangle();
  InitializeEllipse();
  InitializeLine();
}

void Shape::InitializeRectangle() {
  s_RectangleVA = std::make_unique<Core::VertexArray>();

  // 矩形頂點
  s_RectangleVA->AddVertexBuffer(std::make_unique<Core::VertexBuffer>(
      std::vector<float>{
          -0.5f, 0.5f,  // 左上
          -0.5f, -0.5f, // 左下
          0.5f, -0.5f,  // 右下
          0.5f, 0.5f,   // 右上
      },
      2));

  // 索引
  s_RectangleVA->SetIndexBuffer(
      std::make_unique<Core::IndexBuffer>(std::vector<unsigned int>{
          0, 1, 2, // 第一個三角形
          0, 2, 3  // 第二個三角形
      }));
}

void Shape::InitializeCircle(int segments) {
  s_CircleVA = std::make_unique<Core::VertexArray>();

  // 圓形頂點
  std::vector<float> vertices;
  vertices.push_back(0.0f); // 圓心 x
  vertices.push_back(0.0f); // 圓心 y

  for (int i = 0; i <= segments; ++i) {
    float angle = 2.0f * M_PI * i / segments;
    float x = 0.5f * std::cos(angle);
    float y = 0.5f * std::sin(angle);
    vertices.push_back(x);
    vertices.push_back(y);
  }

  s_CircleVA->AddVertexBuffer(
      std::make_unique<Core::VertexBuffer>(vertices, 2));

  // 索引
  std::vector<unsigned int> indices;
  for (int i = 1; i <= segments; ++i) {
    indices.push_back(0); // 圓心
    indices.push_back(i);
    indices.push_back(i < segments ? i + 1 : 1);
  }

  s_CircleVA->SetIndexBuffer(std::make_unique<Core::IndexBuffer>(indices));
}

void Shape::InitializeTriangle() {
  s_TriangleVA = std::make_unique<Core::VertexArray>();

  // 三角形頂點
  s_TriangleVA->AddVertexBuffer(std::make_unique<Core::VertexBuffer>(
      std::vector<float>{
          0.0f, 0.5f,   // 頂點
          -0.5f, -0.5f, // 左下
          0.5f, -0.5f,  // 右下
      },
      2));

  // 索引
  s_TriangleVA->SetIndexBuffer(
      std::make_unique<Core::IndexBuffer>(std::vector<unsigned int>{0, 1, 2}));
}

void Shape::InitializeEllipse(int segments) {
  s_EllipseVA = std::make_unique<Core::VertexArray>();

  // 橢圓頂點
  std::vector<float> vertices;
  vertices.push_back(0.0f); // 圓心 x
  vertices.push_back(0.0f); // 圓心 y

  for (int i = 0; i <= segments; ++i) {
    float angle = 2.0f * M_PI * i / segments;
    float x = 0.5f * std::cos(angle);
    float y = 0.25f * std::sin(angle); // 高度是寬度的一半
    vertices.push_back(x);
    vertices.push_back(y);
  }

  s_EllipseVA->AddVertexBuffer(
      std::make_unique<Core::VertexBuffer>(vertices, 2));

  // 索引
  std::vector<unsigned int> indices;
  for (int i = 1; i <= segments; ++i) {
    indices.push_back(0); // 圓心
    indices.push_back(i);
    indices.push_back(i < segments ? i + 1 : 1);
  }

  s_EllipseVA->SetIndexBuffer(std::make_unique<Core::IndexBuffer>(indices));
}

void Shape::InitializeLine() {
  s_LineVA = std::make_unique<Core::VertexArray>();

  // 線段頂點
  s_LineVA->AddVertexBuffer(std::make_unique<Core::VertexBuffer>(
      std::vector<float>{
          -0.5f, 0.0f, // 起點
          0.5f, 0.0f,  // 終點
          0.5f, 0.01f, // 終點上方
          -0.5f, 0.01f // 起點上方
      },
      2));

  // 索引
  s_LineVA->SetIndexBuffer(std::make_unique<Core::IndexBuffer>(
      std::vector<unsigned int>{0, 1, 2, 0, 2, 3}));
}

} // namespace Util