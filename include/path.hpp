#ifndef PATH_HPP
#define PATH_HPP
#include "collapsible.hpp"
#include <glm/vec2.hpp>
#include <vector>
#include "Util/Position.hpp"

/**
 * @brief 表示遊戲中氣球移動的路徑
 * 
 * 使用向量存儲路徑點，提供一維長度參數到二維位置的映射。
 * 支持碰撞檢測以及路徑長度計算等功能。
 */
class Path {
public:
  /**
   * @brief 建構新路徑
   * @param points 路徑上的點序列
   * @param width 路徑寬度
   */
  Path(std::vector<glm::vec2> points, float width = 0.5f)
      : m_Points(std::move(points)), m_Width(width) {
    calculatePathProperties();
  }

  /**
   * @brief 獲取路徑的總長度（像素）
   * @return 總長度
   */
  [[nodiscard]] float getTotalLength() const { return m_TotalLength; }

  /**
   * @brief 獲取路徑起點
   * @return 起點位置
   */
  [[nodiscard]] Util::PTSDPosition getStartPoint() const {
    return Util::PTSDPosition(m_Points.front().x, m_Points.front().y);
  }

  /**
   * @brief 獲取路徑終點
   * @return 終點位置
   */
  [[nodiscard]] Util::PTSDPosition getEndPoint() const {
    return Util::PTSDPosition(m_Points.back().x, m_Points.back().y);
  }

  /**
   * @brief 從一維參數獲取對應的二維位置
   * @param distance 從路徑起點的距離（像素）
   * @return 對應的位置
   */
  [[nodiscard]] Util::PTSDPosition getPositionAtDistance(float distance) const;

  /**
   * @brief 根據百分比取得位置
   * @param percentage 從0到1的百分比
   * @return 對應的位置
   */
  [[nodiscard]] Util::PTSDPosition getPositionAtPercentage(float percentage) const {
    return getPositionAtDistance(percentage * m_TotalLength);
  }

  /**
   * @brief 根據位置獲取對應的一維距離參數
   * @param position 位置
   * @return 從路徑起點的距離（像素），如果位置不在路徑上返回-1
   */
  [[nodiscard]] float getDistanceAtPosition(const Util::PTSDPosition &position) const;

  /**
   * @brief 根據位置獲取對應的百分比
   * @param position 位置
   * @return 從0到1的百分比，如果位置不在路徑上返回-1
   */
  [[nodiscard]] float getPercentageAtPosition(const Util::PTSDPosition &position) const {
    float distance = getDistanceAtPosition(position);
    return distance < 0 ? -1 : distance / m_TotalLength;
  }

  /**
   * @brief 檢查位置是否在路徑上
   * @param position 要檢查的位置
   * @return 是否在路徑上
   */
  [[nodiscard]] bool isOnPath(const Util::PTSDPosition &position) const;

  /**
   * @brief 獲取路徑的所有點
   * @return 路徑點向量的常量引用
   */
  [[nodiscard]] const std::vector<glm::vec2>& getPoints() const { return m_Points; }

  /**
   * @brief 獲取路徑寬度
   * @return 路徑寬度
   */
  [[nodiscard]] float getWidth() const { return m_Width; }

private:
  std::vector<glm::vec2> m_Points;  // 路徑點
  float m_Width;                    // 路徑寬度
  float m_TotalLength = 0.0f;       // 預計算的總長度
  std::vector<float> m_SegmentLengths;  // 每段路徑的長度
  std::vector<float> m_CumulativeLengths;  // 累積長度

  /**
   * @brief 計算路徑屬性（總長度和段長度）
   */
  void calculatePathProperties();

  /**
   * @brief 找到距離對應的路徑段
   * @param distance 從起點的距離
   * @return 段索引和該段內的距離
   */
  [[nodiscard]] std::pair<size_t, float> findSegmentAtDistance(float distance) const;
};

#endif // PATH_HPP