// ShapeAnimation.cpp
#include "core/ShapeAnimation.hpp"
#include "Util/Logger.hpp"

namespace Util {

ShapeAnimation::ShapeAnimation(const std::vector<std::shared_ptr<Shape>> &shapes,
                               bool play, std::size_t interval, bool looping,
                               std::size_t cooldown)
    : m_State(play ? State::PLAY : State::PAUSE), 
      m_Interval(interval),
      m_Looping(looping), 
      m_Cooldown(cooldown) {
    
    m_Frames = shapes; // 直接複製所有幀
}

ShapeAnimation::ShapeAnimation(ShapeType type,
                              const std::vector<glm::vec2> &sizes,
                              const std::vector<Color> &colors,
                              bool play, std::size_t interval, 
                              bool looping, std::size_t cooldown)
    : m_State(play ? State::PLAY : State::PAUSE),
      m_Interval(interval),
      m_Looping(looping),
      m_Cooldown(cooldown) {
    
    // 確保尺寸和顏色向量長度相同
    size_t frameCount = std::min(sizes.size(), colors.size());
    
    // 創建所有形狀幀
    for (size_t i = 0; i < frameCount; i++) {
        auto shape = std::make_shared<Shape>(type, sizes[i], colors[i]);
        m_Frames.push_back(shape);
    }
}

void ShapeAnimation::SetCurrentFrame(std::size_t index) {
    if (index >= m_Frames.size()) {
        LOG_ERROR("ShapeAnimation: Frame index out of bounds: {}", index);
        return;
    }
    
    m_Index = index;
    if (m_State == State::ENDED || m_State == State::COOLDOWN) {
        m_IsChangeFrame = true;
    }
}

void ShapeAnimation::Play() {
    if (m_State == State::PLAY)
        return;
    
    if (m_State == State::ENDED || m_State == State::COOLDOWN) {
        m_Index = m_IsChangeFrame ? m_Index : 0;
        m_IsChangeFrame = false;
    }
    
    m_State = State::PLAY;
    LOG_DEBUG("ShapeAnimation: Playing from frame {}", m_Index);
}

void ShapeAnimation::Pause() {
    if (m_State == State::PLAY || m_State == State::COOLDOWN) {
        m_State = State::PAUSE;
        LOG_DEBUG("ShapeAnimation: Paused at frame {}", m_Index);
    }
}

void ShapeAnimation::Draw(const Core::Matrices &data) {
    // 添加調試日誌
    LOG_DEBUG("ShapeAnimation: Drawing frame {} of {}",
             m_Index, m_Frames.size());
    
    // 渲染當前幀
    if (!m_Frames.empty() && m_Index < m_Frames.size()) {
        m_Frames[m_Index]->Draw(data);
    } else {
        LOG_ERROR("ShapeAnimation: No frames to draw or invalid index");
    }
    
    // 更新動畫
    Update();
}

void ShapeAnimation::Update() {
    unsigned long nowTime = Util::Time::GetElapsedTimeMs();
    
    // 處理暫停和結束狀態
    if (m_State == State::PAUSE || m_State == State::ENDED) {
        return;
    }
    
    // 處理冷卻狀態
    if (m_State == State::COOLDOWN) {
        if (nowTime >= m_CooldownEndTime) {
            Play();
        }
        return;
    }
    
    // 更新時間累積
    m_TimeBetweenFrameUpdate += Util::Time::GetDeltaTimeMs();
    auto updateFrameCount = static_cast<unsigned int>(m_TimeBetweenFrameUpdate / m_Interval);
    
    if (updateFrameCount <= 0)
        return;
    
    // 更新幀索引
    m_Index += updateFrameCount;
    m_TimeBetweenFrameUpdate = 0;
    
    // 處理幀索引超出範圍的情況
    unsigned int totalFrames = m_Frames.size();
    if (m_Index >= totalFrames) {
        if (m_Looping) {
            // 設置冷卻結束時間
            m_CooldownEndTime = nowTime + m_Cooldown;
            m_State = State::COOLDOWN;
            m_Index = m_Index % totalFrames; // 循環索引
        } else {
            // 結束動畫
            m_State = State::ENDED;
            m_Index = totalFrames - 1; // 停在最後一幀
        }
    }
}

glm::vec2 ShapeAnimation::GetSize() const {
    if (m_Frames.empty()) {
        return glm::vec2(0.0f, 0.0f);  // 如果沒有幀，返回零大小
    }
    
    // 返回當前幀的大小
    if (m_Index < m_Frames.size()) {
        return m_Frames[m_Index]->GetSize();
    }
    
    // 如果索引超出範圍，返回第一幀的大小
    return m_Frames.front()->GetSize();
}
} // namespace Util