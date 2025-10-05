#include "Window.h"

#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

namespace Mineclone {

    void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }



    void APIENTRY Window::messageCallback(GLenum source, GLenum type, GLuint id,
                                  GLenum severity, GLsizei length,
                                  const GLchar* message, const void* userParam) {
        std::cerr << "GL CALLBACK: " << message << std::endl;
    }

    Window::Window(const std::string& title, int width, int height) {
        if(!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
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
        glfwSwapInterval(1);
        glViewport(0, 0, width, height);

        glfwSetFramebufferSizeCallback(m_handle, framebufferSizeCallback);

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(Window::messageCallback, 0);
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(m_handle, true);
        ImGui_ImplOpenGL3_Init("#version 330");
        ImGui::StyleColorsDark();
    }

    Window::~Window() {
        glfwTerminate();
    }

    std::pair<int, int> Window::getSize() const {
        int width, height;
        glfwGetFramebufferSize(m_handle, &width, &height);
        return {width, height};
    }

    bool Window::shouldClose() {
        return glfwWindowShouldClose(m_handle);
    }

    void Window::swapBuffers() {
        glfwSwapBuffers(m_handle);
    }

    bool Window::isCursorCaptured() const {
        return m_isCursorCaptured;
    }

    void Window::captureCursor() {
        glfwSetInputMode(m_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        m_isCursorCaptured = true;
    }

    void Window::releaseCursor() {
        glfwSetInputMode(m_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        m_isCursorCaptured = false;
    }

    std::pair<float, float> Window::getCurrentMousePosition() const {
        double mouseX, mouseY;
        glfwGetCursorPos(m_handle, &mouseX, &mouseY);
        return std::pair(mouseX, mouseY);
    }

    bool Window::isKeyPressed(int key) const {
        return glfwGetKey(m_handle, key) == GLFW_PRESS;
    }




}