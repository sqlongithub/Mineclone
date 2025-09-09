#include <stdexcept>
#include <iostream>
#include "Application.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace Mineclone {

    Application::Application(const std::string &title)
        : m_window("Mineclone", 1280, 960) {

        if(!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw std::runtime_error("Failed to initialize GLAD");
        }


    }
}