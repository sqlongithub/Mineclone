#include <stdexcept>
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Application.h"


namespace Mineclone {

    Application::Application(const std::string &title)
        : m_window("Mineclone", 1280, 960),
          m_renderer(),
          m_game() {

    }

    void Application::run() {
        while(!m_window.shouldClose())
        {
            glfwPollEvents();
            m_renderer.beginFrame();
            m_game.draw(m_renderer);
            m_renderer.endFrame(m_window);
        }
    }
}