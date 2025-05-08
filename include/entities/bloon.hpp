#ifndef BLOON_HPP
#define BLOON_HPP

#include "Util/GameObject.hpp"
#include "Util/Position.hpp"
#include "components/collisionComp.hpp"
#include "interfaces/clickable.hpp"
#include "components/mortal.hpp"
#include <memory>
#include <vector>

class Bloon final : public Util::GameObject, 
                   public Interface::I_clickable, 
                   public Mortal,
                   public Components::CollisionComponent {
public:
    enum class State { alive, frozed, glued, pop, died };
    enum class Type { red, blue, green, yellow, black, white, lead, rainbow };

private:
    Bloon::State m_CurrentState = Bloon::State::alive;
    Type m_Type;
    State m_State;
    const float baseSpeed = 2.5f; // 氣球基礎速度
    float m_SpeedMult;            // 實際速度 = baseSpeed*m_SpeedMult
    int m_RBE;                    // Red Bloon Equivalent (用於生命值計算)
    int freeze_counter = 0; // 冰凍計時器
    std::vector<std::shared_ptr<Bloon::Type>> m_ChildBloons;
    
    // 碰撞組件
    std::shared_ptr<Components::CollisionComponent> m_collisionComponent;
    bool m_clickable = true;

public:
    Bloon(Type type, Util::PTSDPosition pos, float z_index);
    ~Bloon() override = default;

    // 碰撞相關方法
    void setPosition(const Util::PTSDPosition& position) override;
    Util::PTSDPosition getPosition() const override;


    void updateFreeze();
    void setFrozed(int freeze_frame);
    void set_died() { m_State = State::died; }
    State GetCurrentState() const { return m_CurrentState; }
    float GetSpeed() const { return (m_State==Bloon::State::alive)?(baseSpeed * m_SpeedMult):(0.0f); }
    int GetRBE() const { return m_RBE; }
    Type GetType() const { return m_Type; }
    State GetState() const { return m_State; }
    std::vector<std::shared_ptr<Bloon::Type>> GetChildBloons() const { return m_ChildBloons; }
    
    // I_clickable 介面實現
    void onClick() override;
    void onFocus() override {}
    bool isClickable() const override { return m_clickable; }
    void setClickable(bool clickable) override { m_clickable = clickable; }
};

#endif // BLOON_HPP