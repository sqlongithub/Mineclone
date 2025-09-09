
#include <stdexcept>
#include "glad/glad.h"

#include "Renderer.h"


namespace Mineclone {

    Renderer::Renderer() {

    }

    void Renderer::beginFrame() {
        glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::endFrame(Window &window) {
        window.swapBuffers();
    }

}