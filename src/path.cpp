#include "path.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

void Path::calculatePathProperties() {
  m_TotalLength = 0.0f;
  m_SegmentLengths.clear();
  m_CumulativeLengths.clear();
  
  if (m_Points.size() < 2) {
    return;  // 路徑至少需要兩個點
  }
  
  m_CumulativeLengths.push_back(0.0f);  // 起點累積長度為0
  
  for (size_t i = 1; i < m_Points.size(); ++i) {
    const auto& p1 = m_Points[i-1];
    const auto& p2 = m_Points[i];
    
    // 計算兩點之間的距離
    float segmentLength = std::sqrt(std::pow(p2.x - p1.x, 2) + std::pow(p2.y - p1.y, 2));
    m_SegmentLengths.push_back(segmentLength);
    m_TotalLength += segmentLength;
    m_CumulativeLengths.push_back(m_TotalLength);
  }
}

std::pair<size_t, float> Path::findSegmentAtDistance(float distance) const {
  if (distance < 0 || distance > m_TotalLength) {
    throw std::out_of_range("Distance is outside path bounds");
  }
  
  // 使用二分查找定位段
  auto it = std::upper_bound(m_CumulativeLengths.begin(), m_CumulativeLengths.end(), distance);
  size_t segmentIndex = std::distance(m_CumulativeLengths.begin(), it) - 1;
  
  // 計算段內的局部距離
  float segmentPosition = distance - m_CumulativeLengths[segmentIndex];
  
  return {segmentIndex, segmentPosition};
}

Util::PTSDPosition Path::getPositionAtDistance(float distance) const {
  if (m_Points.empty()) {
    throw std::runtime_error("Path has no points");
  }
  
  if (distance <= 0) {
    return Util::PTSDPosition(m_Points.front().x, m_Points.front().y);
  }
  
  if (distance >= m_TotalLength) {
    return Util::PTSDPosition(m_Points.back().x, m_Points.back().y);
  }
  
  // 找到對應的段和段內位置
  auto [segmentIndex, segmentPosition] = findSegmentAtDistance(distance);
  
  // 計算段內的百分比
  float segmentPercent = segmentPosition / m_SegmentLengths[segmentIndex];
  
  // 線性插值計算實際位置
  const auto& p1 = m_Points[segmentIndex];
  const auto& p2 = m_Points[segmentIndex + 1];
  
  float x = p1.x + (p2.x - p1.x) * segmentPercent;
  float y = p1.y + (p2.y - p1.y) * segmentPercent;
  
  return Util::PTSDPosition(x, y);
}

float Path::getDistanceAtPosition(const Util::PTSDPosition &position) const {
  if (m_Points.empty()) {
    return -1;
  }
  
  // 檢查位置是否在路徑上，並找到最近的點
  float minDistance = std::numeric_limits<float>::max();
  float resultDistance = -1;
  
  // 檢查每個路徑段
  for (size_t i = 0; i < m_Points.size() - 1; ++i) {
    const auto& p1 = m_Points[i];
    const auto& p2 = m_Points[i + 1];
    
    // 向量計算 - 使用點到線段的距離公式
    glm::vec2 segmentVector = p2 - p1;
    glm::vec2 positionVector = {position.x - p1.x, position.y - p1.y};
    
    float segmentLength = glm::length(segmentVector);
    float dotProduct = glm::dot(positionVector, segmentVector) / segmentLength;
    
    // 計算點到線段的投影位置
    float t = std::max(0.0f, std::min(segmentLength, dotProduct)) / segmentLength;
    
    glm::vec2 projection = p1 + t * segmentVector;
    float distance = glm::distance(glm::vec2(position.x, position.y), projection);
    
    // 如果點在路徑寬度範圍內，計算從起點的距離
    if (distance <= m_Width && distance < minDistance) {
      minDistance = distance;
      resultDistance = m_CumulativeLengths[i] + t * m_SegmentLengths[i];
    }
  }
  
  return resultDistance;
}

bool Path::isOnPath(const Util::PTSDPosition &position) const {
  return getDistanceAtPosition(position) >= 0;
}