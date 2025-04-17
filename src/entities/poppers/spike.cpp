#include "entities/poppers/spike.hpp"
#include "Util/Image.hpp"
#include "Util/Position.hpp"
spike::spike(const Util::PTSDPosition &pos)
    : popper(pos, 10.0f),
      m_object(std::make_shared<Util::GameObject>(
          std::make_shared<Util::Image>(RESOURCE_DIR "/poppers/spike.png"), 5.0f)) {
            m_object->SetVisible(true);
            m_object->m_Transform.translation = glm::vec2(pos.x, pos.y);
            m_position = pos;
          }

std::vector<bool> spike::hit(std::vector<std::shared_ptr<Bloon>> bloons) {
  std::vector<bool> hit_results(bloons.size(), false);

  for (size_t i = 0; i < bloons.size(); i++) {
    // 如果 spike 還有生命值，可以擊破氣球
    if (this->life > 0) {
      hit_results[i] = true; // 標記此氣球會被擊破
      this->life--;          // 減少 spike 的生命值

      // 如果 spike 生命值歸零，標記為死亡並跳出循環
      if (this->life <= 0) {
        this->kill();
        break; // 生命值用完，無法再擊破更多氣球
      }
    } else {
      // 生命值已用完，無法擊破更多氣球
      hit_results[i] = false;
    }
  }

  return hit_results;
}
std::shared_ptr<Util::GameObject> spike::get_object() { return m_object; }