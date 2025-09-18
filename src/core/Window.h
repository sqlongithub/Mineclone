//
// Created by Niclas on 07/09/2025.
//

#pragma once

#include <string>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Dispatcher.h"

namespace Mineclone {

    class Window {
    public:


        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(const std::string& title, int width, int height, Dispatcher& dispatcher);
        ~Window();

        bool shouldClose();
        void swapBuffers();
        [[nodiscard]] std::pair<float, float> getCurrentMousePosition() const;
        [[nodiscard]] bool isKeyPressed(int key) const;

        void captureCursor();
        void releaseCursor();
        bool isCursorCaptured() const;
    private:
        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
        GLFWwindow* m_handle;

        bool m_isCursorCaptured = false;

        static void
        messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,
                        const void *userParam);
    };

} // Mineclone

