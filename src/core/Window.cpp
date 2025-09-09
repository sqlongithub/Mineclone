#include <iostream>
#include "Window.h"
#include "GLFW/glfw3.h"

namespace Mineclone {

    void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    Window::Window(const std::string& title, int width, int height) {
        m_handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (m_handle == nullptr)
        {
            throw std::runtime_error("Failed to create window");
        }
        glfwMakeContextCurrent(m_handle);
        glViewport(0, 0, width, height);

        glfwSetFramebufferSizeCallback(m_handle, framebufferSizeCallback);
    }

    Window::~Window() {
        glfwTerminate();
    }


}