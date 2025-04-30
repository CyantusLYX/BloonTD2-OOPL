#include "UI/SidebarManager.hpp"
#include "Util/Logger.hpp"
#include "Util/Renderer.hpp"
#include "core/shape.hpp"

namespace UI {

SidebarManager::SidebarManager(const Util::PTSDPosition &position, float height,
                               float width, float zIndex)
    : m_position(position), m_size(width, height), m_zIndex(zIndex) {

  // 創建背景形狀
  m_backgroundShape =
      std::make_shared<Util::Shape>(Util::ShapeType::Rectangle, m_size);
  m_backgroundShape->SetColorRGB(255, 255,255,100); // 半透明灰色背景

  // 創建背景GameObject
  m_background = std::make_shared<Util::GameObject>(m_backgroundShape, zIndex);

  // 設置背景位置 (相對於畫面中心的絕對坐標)
  m_background->m_Transform.translation = position.ToVec2();

  // 計算各區域高度比例
  float statusHeight = height * 0.12f; // 12% 給狀態區
  float buyHeight = height * 0.38f;    // 38% 給購買區
  float infoHeight = height * 0.50f;   // 50% 給Flag區

  // 計算初始位置 - 直接計算正確位置而不是設為 (0,0)
  float topY = position.y + height / 2.0f;
  float statusBarY = topY - statusHeight / 2.0f - 5.0f;
  float towerBtnPanelY =
      statusBarY - statusHeight / 2.0f - buyHeight / 2.0f - 5.0f;

  // 創建狀態列元件 - 使用計算好的位置
  m_statusBar = std::make_shared<StatusBar>(
      Util::PTSDPosition(position.x, statusBarY), // 修正：直接使用計算好的位置
      width - 10.0f,                              // 減去一些間距
      statusHeight, zIndex + 0.1f,
      true // 固定大小
  );

  // 創建塔按鈕面板元件 - 使用計算好的位置
  m_towerBtnPanel = std::make_shared<TowerButtonsPanel>(
      Util::PTSDPosition(position.x,
                         towerBtnPanelY), // 修正：直接使用計算好的位置
      width - 10.0f,                      // 減去一些間距
      buyHeight, zIndex + 0.1f);

  // 暫時不創建具體Flag實例
  m_infoFlag = nullptr;

  LOG_DEBUG(
      "SidebarManager: Created with dimensions {}x{} at position ({}, {})",
      width, height, position.x, position.y);
}

void SidebarManager::setRenderer(
    const std::shared_ptr<Util::Renderer> &renderer) {
  m_renderer = renderer;

  // 添加所有元素到渲染器
  if (auto r = m_renderer.lock()) {
    r->AddChild(m_background);
    r->AddChild(m_statusBar);
    r->AddChild(m_towerBtnPanel);
    if (m_infoFlag) {
      r->AddChild(m_infoFlag);
    }

    // 設置完渲染器後重新計算位置
    recalculatePanelPositions();
  } else {
    LOG_ERROR(
        "SidebarManager: Failed to set renderer - renderer is null or expired");
  }
}

void SidebarManager::recalculatePanelPositions() {
  // 重要：所有位置都是相對於畫面中心的絕對坐標

  // 獲取側邊欄的頂部和底部位置
  float topY = m_position.y + m_size.y / 2.0f;
  float centerX = m_position.x; // 側邊欄中心X座標

  // StatusBar位置 - 頂部
  float statusBarHeight = m_statusBar->getSize().y;
  float statusBarY = topY - statusBarHeight / 2.0f - 5.0f;
  m_statusBar->setPosition(Util::PTSDPosition(centerX, statusBarY));

  // TowerButtonPanel位置 - 在StatusBar下方
  float towerBtnPanelHeight = m_towerBtnPanel->getSize().y;
  float towerBtnPanelY =
      statusBarY - statusBarHeight / 2.0f - towerBtnPanelHeight / 2.0f - 5.0f;
  m_towerBtnPanel->setPosition(Util::PTSDPosition(centerX, towerBtnPanelY));

  // InfoFlag位置 - 在TowerButtonPanel下方 (如果存在)
  if (m_infoFlag) {
    float flagHeight = m_infoFlag->getSize().y;
    float flagY =
        towerBtnPanelY - towerBtnPanelHeight / 2.0f - flagHeight / 2.0f - 5.0f;
    m_infoFlag->setPosition(Util::PTSDPosition(centerX, flagY));
  }

  LOG_DEBUG("SidebarManager: Panel positions recalculated - StatusBar at y={}, "
            "TowerBtnPanel at y={}",
            statusBarY, towerBtnPanelY);
}

void SidebarManager::setPosition(const Util::PTSDPosition &position) {
  m_position = position;

  // 更新背景位置
  m_background->m_Transform.translation = position.ToVec2();

  // 重新計算並設置各面板位置
  recalculatePanelPositions();

  LOG_DEBUG("SidebarManager: Position set to ({}, {})", position.x, position.y);
}

void SidebarManager::setSize(const glm::vec2 &size) {
  m_size = size;

  // 更新背景大小
  m_backgroundShape->SetSize(size);

  // 重新計算各面板大小
  float statusHeight = size.y * 0.12f;
  float buyHeight = size.y * 0.38f;
  float infoHeight = size.y * 0.50f;

  // 更新各面板大小
  m_statusBar->setSize(glm::vec2(size.x - 10.0f, statusHeight));
  m_towerBtnPanel->setSize(glm::vec2(size.x - 10.0f, buyHeight));

  if (m_infoFlag) {
    m_infoFlag->setSize(glm::vec2(size.x - 10.0f, infoHeight));
  }

  // 重新計算位置
  recalculatePanelPositions();

  LOG_DEBUG("SidebarManager: Size updated to {}x{}", size.x, size.y);
}

void SidebarManager::show(bool visible) {
  m_background->SetVisible(visible);
  m_statusBar->SetVisible(visible);
  m_towerBtnPanel->SetVisible(visible);
  // Flag 的可見性不在此處控制

  LOG_DEBUG("SidebarManager: Visibility set to {}", visible);
}

// 委託給StatusBar的方法
void SidebarManager::updateMoney(int amount) {
  if (m_statusBar) {
    m_statusBar->updateMoney(amount);
  }
}

void SidebarManager::updateLives(int amount) {
  if (m_statusBar) {
    m_statusBar->updateLives(amount);
  }
}

// 委託給TowerButtonsPanel的方法
void SidebarManager::addTowerButton(
    const std::shared_ptr<TowerButton> &button) {
  if (m_towerBtnPanel) {
    m_towerBtnPanel->addTowerButton(button);
  }
}

std::vector<std::shared_ptr<TowerButton>>
SidebarManager::getAllTowerButtons() const {
  if (m_towerBtnPanel) {
    return m_towerBtnPanel->getAllButtons();
  }
  return {};
}

void SidebarManager::clearTowerButtons() {
  if (m_towerBtnPanel) {
    m_towerBtnPanel->clearButtons();
  }
}

} // namespace UI