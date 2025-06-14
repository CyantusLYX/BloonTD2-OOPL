#ifndef APP_HPP
#define APP_HPP

#include "Util/GameObject.hpp"
#include "Util/Position.hpp"
#include "Util/Renderer.hpp"
#include "core/manager.hpp"
#include "core/shape.hpp"
#include "glm/fwd.hpp"
#include "pch.hpp" // IWYU pragma: export
#include <memory>

class App {
public:
  enum class State {
    START,
    UPDATE,
    END,
  };

  [[nodiscard]]
  State GetCurrentState() const {
    return m_CurrentState;
  }

  void Start();

  void Update();

  void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
  void ValidTask();
  std::shared_ptr<Util::Renderer> m_Renderer =
      std::make_shared<Util::Renderer>();
  std::shared_ptr<Manager> manager = std::make_shared<Manager>(m_Renderer);

private:
  State m_CurrentState = State::START;
  Util::PTSDPosition previous_cursor_pos = {0, 0};
};

#endif
