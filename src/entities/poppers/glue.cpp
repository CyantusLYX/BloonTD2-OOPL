#include "entities/poppers/glue.hpp"
#include "Util/Image.hpp"
#include "Util/Logger.hpp"
#include "Util/Position.hpp"
#include "components/canBuy.hpp"
#include "conf.hpp"
#include "config.hpp"

Glue::Glue(const Util::PTSDPosition &pos)
    : popper(pos, 15.0f), CanBuy(Tower::TowerType::glue, COST_GLUE),
      m_object(std::make_shared<Util::GameObject>(
          std::make_shared<Util::Image>(RESOURCE_DIR "/poppers/glue.png"), 5.0f)),
      m_position(pos.x, pos.y) {
    m_object->SetVisible(true);
    m_object->m_Transform.translation = glm::vec2(pos.x, pos.y);
}

std::vector<bool> Glue::hit(std::vector<std::shared_ptr<Bloon>> bloons) {
    std::vector<bool> hit_results(bloons.size(), false);

    for (size_t i = 0; i < bloons.size(); i++) {
        // 如果膠水還有生命值，可以減慢氣球速度
        if (this->life > 0) {
            auto bloon = bloons[i];
            
            // 將氣球狀態設為黏滯 (Glued)
            if (bloon->GetState() == Bloon::State::alive) { // 只對正常狀態的氣球有效
                bloon->setGlued();
                this->life--; // 減少膠水的生命值
                
                // 標記此氣球已被處理，但不會被破壞 (所以返回 false)
                hit_results[i] = false;
                
                LOG_INFO("GLUE : 氣球被黏住：位置 ({}, {})", bloon->getPosition().x, bloon->getPosition().y);
            }
            
            // 如果膠水生命值歸零，標記為死亡並跳出循環
            if (this->life <= 0) {
                this->kill();
                LOG_INFO("GLUE : 膠水用完：位置 ({}, {})", m_position.x, m_position.y);
                break;
            }
        } else {
            // 生命值已用完，無法再影響更多氣球
            hit_results[i] = false;
        }
    }

    // 返回 hit_results（全部是 false，因為膠水不破壞氣球）
    return hit_results;
}

std::shared_ptr<Util::GameObject> Glue::get_object() { 
    return m_object; 
}

void Glue::onDragStart() {
    // 開始拖曳時的處理
    LOG_INFO("GLUE : 開始拖曳膠水");
}

void Glue::onDrag(const Util::PTSDPosition& newPosition) {
    // 更新膠水的位置
    m_object->m_Transform.translation = newPosition.ToVec2();
    m_position = newPosition;
    // 更新碰撞檢測位置
    CollisionComponent::setPosition(newPosition);
}

void Glue::onDragEnd() {
    // 結束拖曳時的處理
    LOG_INFO("GLUE : 膠水放置完成，位置: ({}, {})", m_position.x, m_position.y);
    // 放置後不再可拖曳
    m_draggable = false;
}
