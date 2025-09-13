
#include "glad/glad.h"
#include "Renderer.h"
#include "Shader.h"
#include "Mesh.h"
#include "glm/gtx/string_cast.hpp"

namespace Mineclone {



    Renderer::Renderer() {
        glEnable(GL_DEPTH_TEST);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void Renderer::beginFrame() {
        glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::endFrame(Window &window) {
        window.swapBuffers();

    }



    void Renderer:: drawMesh(Mesh& mesh, const Shader& shader, const glm::mat4& mvp)
    {
        shader.bind();
        checkGLError("shader.bind");

        shader.setUniformMat4("u_MVP", mvp);
        checkGLError("shader.setUniformMat4");

        //std::cout << "MVP = " << glm::to_string(mvp) << std::endl;
        mesh.draw();
        checkGLError("mesh.draw");
    }

}