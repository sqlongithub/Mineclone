#pragma once

#include "../core/Window.h"
#include "assets/AssetManager.h"
#include "glm/common.hpp"
#include "render_queue/RenderCamera.h"
#include "materials/Shader.h"
#include "render_queue/RenderQueue.h"
#include "gpu/FrameData.h"
#include "gpu/UniformBuffer.h"

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
        void beginFrame(const RenderCamera& camera);
        void endFrame(Window& window);

        void submit(const RenderCommand& cmd);
        void flushQueue(const AssetManager& assets);

        void setClearColor(const glm::vec4& color) {
            m_clearColor.r = glm::clamp(color.r, 0.0f, 1.0f);
            m_clearColor.g = glm::clamp(color.g, 0.0f, 1.0f);
            m_clearColor.b = glm::clamp(color.b, 0.0f, 1.0f);
            m_clearColor.a = glm::clamp(color.a, 0.0f, 1.0f);
        }

        [[nodiscard]] glm::vec4 getClearColor() const { return m_clearColor; }

        static RenderCommand& getLastCommandLastFrame();

    private:
        static void executeCommand(const RenderCommand& cmd, const AssetManager& assets);
        
        RenderQueue m_renderQueue;
        FrameData m_frameData{};
        UniformBuffer m_frameUBO;
        glm::vec4 m_clearColor {0.5, 0.7, 1.0, 0};

        static RenderCommand m_lastCommandLastFrame;

    };
}
