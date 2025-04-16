#ifndef MORTAL_HPP
#define MORTAL_HPP
#include <string>
class Mortal {
public:
    enum class LifeStatus { Alive, Dead };

    Mortal();
    virtual ~Mortal() = default;

    // 生命狀態管理
    bool is_alive() const { return m_life_status == LifeStatus::Alive; }
    bool is_dead() const { return m_life_status == LifeStatus::Dead; }
    void kill();
    virtual void pre_kill(){};
    
    // 獲取唯一識別碼
    const std::string& get_uuid() const { return m_uuid; }

protected:
    LifeStatus m_life_status = LifeStatus::Alive;

private:
    std::string m_uuid;  // 使用UUID作為唯一識別符
};

#endif // MORTAL_HPP