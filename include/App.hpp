#ifndef APP_HPP
#define APP_HPP

#include "Util/Renderer.hpp"
#include "manager.hpp"
#include "pch.hpp" // IWYU pragma: export

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    void ValidTask();
    Util::Renderer m_Renderer;
    Manager manager;
private:
    State m_CurrentState = State::START;
};

#endif
