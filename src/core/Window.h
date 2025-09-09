//
// Created by Niclas on 07/09/2025.
//

#pragma once

#include <string>
#include "GLFW/glfw3.h"

namespace Mineclone {

    class Window {
    public:
        Window(const std::string& title, int width, int height);
        ~Window();

        bool shouldClose();
        void swapBuffers();
    private:
        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
        GLFWwindow* m_handle;
    };

} // Mineclone

