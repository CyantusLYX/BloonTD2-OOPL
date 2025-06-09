#ifndef MORTAL_HPP
#define MORTAL_HPP
#include <memory>
#include <string>
#include <vector>
class Mortal {
private:
  std::vector<std::shared_ptr<Mortal>> m_MortalChildren;

public:
  enum class LifeStatus { Alive, Dead };

  Mortal();
  virtual ~Mortal() = default;

  // 生命狀態管理
  void add_child(std::shared_ptr<Mortal> child) {
    m_MortalChildren.push_back(child);
  }
  bool is_alive() const { return m_life_status == LifeStatus::Alive; }
  bool is_dead() const { return m_life_status == LifeStatus::Dead; }
  std::vector<std::shared_ptr<Mortal>> get_children() {
    return m_MortalChildren;
  }
  void kill();
  void kill_children() {
    for (const auto &child : m_MortalChildren) {
      if (child) {
        child->kill();
      }
    }
    m_MortalChildren.clear();
  }
  virtual void pre_kill() {
  };

  // 獲取唯一識別碼
  const std::string &get_uuid() const { return m_uuid; }

protected:
  LifeStatus m_life_status = LifeStatus::Alive;

private:
  std::string m_uuid; // 使用UUID作為唯一識別符
};

#endif // MORTAL_HPP