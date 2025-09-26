#include <stdexcept>
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Application.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "game/input/InputActionSystem.h"
#include "game/systems/CursorSystem.h"

namespace Mineclone {

    Application::Application(const std::string &title)
        : m_window("Mineclone", 2160, 1215, m_dispatcher),
          m_game(m_registry, m_systemManager, m_dispatcher) {

        m_systemManager.addSystem<InputActionSystem>(m_window, m_contextManager);
        m_systemManager.addSystem<CursorSystem>(m_window);

        m_game.initSystems();
    }

    void Application::run() {
        double lastTime = glfwGetTime();
        while(!m_window.shouldClose())
        {
            double currentTime = glfwGetTime();
            auto dt = static_cast<float>(currentTime - lastTime); // seconds
            lastTime = currentTime;

            glfwPollEvents();

            m_game.update(dt);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            m_game.draw(m_renderer, dt);
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            m_renderer.endFrame(m_window);


        }
    }
}
