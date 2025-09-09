#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Window.h"

namespace Mineclone {

    void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    Window::Window(const std::string& title, int width, int height) {
        if(!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        m_handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (m_handle == nullptr)
        {
            throw std::runtime_error("Failed to create window");
        }
        glfwMakeContextCurrent(m_handle);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw std::runtime_error("Failed to initialize GLAD");
        }
        glViewport(0, 0, width, height);

        glfwSetFramebufferSizeCallback(m_handle, framebufferSizeCallback);
    }

    Window::~Window() {
        glfwTerminate();
    }

    bool Window::shouldClose() {
        return glfwWindowShouldClose(m_handle);
    }

    void Window::swapBuffers() {
        glfwSwapBuffers(m_handle);
    }


}