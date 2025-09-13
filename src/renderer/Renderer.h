#pragma once

#include "../core/Window.h"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/common.hpp"
#include "RenderCamera.h"
#include "Registry.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"

inline void checkGLError(const std::string& location) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "[OpenGL Error] " << location << ": " << err << std::endl;
    }
}

namespace Mineclone {


    class Renderer {
    public:
        Renderer();
        ~Renderer() = default;
        void beginFrame();
        void endFrame(Window& window);

        void setClearColor(const glm::vec4& color) {
            m_clearColor.r = glm::clamp(color.r, 0.0f, 1.0f);
            m_clearColor.g = glm::clamp(color.g, 0.0f, 1.0f);
            m_clearColor.b = glm::clamp(color.b, 0.0f, 1.0f);
            m_clearColor.a = glm::clamp(color.a, 0.0f, 1.0f);
        }

        [[nodiscard]] glm::vec4 getClearColor() const { return m_clearColor; }

        void drawMesh(Mineclone::Mesh &mesh, const Mineclone::Shader &shader, const glm::mat4 &mvp);

    private:
        glm::vec4 m_clearColor {0.23, 0.3, 0.3, 0};

    };
}
