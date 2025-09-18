
#include "glad/glad.h"
#include "Renderer.h"
#include "materials/Shader.h"
#include "mesh/Mesh.h"
#include "glm/gtx/string_cast.hpp"
#include "AssetManager.h"

namespace Mineclone {



    Renderer::Renderer()
        : m_renderQueue(),
          m_frameUBO(sizeof(FrameData), 0) {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        //glFrontFace(GL_CW);
        glCullFace(GL_BACK);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void Renderer::beginFrame(const RenderCamera& camera) {
        m_frameData.viewProjection = camera.viewProjection;
        m_frameData.cameraPosition = camera.position;
        m_frameUBO.setData(&m_frameData, sizeof(FrameData));

        glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::endFrame(Window &window) {
        window.swapBuffers();
    }

    void Renderer::flushQueue(const AssetManager& assets) {
        // Iterate layers in order
        for (size_t i = 0; i < static_cast<size_t>(Layer::LAYER_COUNT); i++) {
            auto layer = static_cast<Layer>(i);
            auto& bucket = m_renderQueue.getBucket(static_cast<Layer>(i));

            // Transparent sorting
            if (layer == Layer::TRANSPARENT) {
                glDepthMask(GL_FALSE);
                std::sort(bucket.begin(), bucket.end(), [this](const RenderCommand& a, const RenderCommand& b){
                    return a.getDistanceToCamera(m_frameData.cameraPosition) > b.getDistanceToCamera(m_frameData.cameraPosition);
                });
                glDepthMask(GL_TRUE);
            }

            for (auto& cmd : bucket) {
                executeCommand(cmd, assets);
            }
        }
        m_renderQueue.clear();
    }

    void Renderer::submit(const Mineclone::RenderCommand& cmd) {
        m_renderQueue.add(cmd);
    }

    void Renderer::executeCommand(const RenderCommand& cmd, const AssetManager& assets) {
        if (!cmd.mesh || !cmd.material.isValid())
            return;

        const Material* material = assets.materials().get(cmd.material);
        if (!material || !material->isValid()) {
            std::cout << "invalid material" << std::endl;
            return;
        };

        const Shader* shader = assets.shaders().get(material->shader);
        if (!shader) return;

        shader->bind();

        material->textures->bind(0);
        shader->setUniformInt("u_TextureArray", 0);

        shader->setUniformMat4("u_Model", cmd.modelMatrix);


        //shader->setUniformInt("u_Texture", 0); // albedo
        //shader->setUniformInt("u_Normal", 1);  // normal

        cmd.mesh->getVAO().bind();
        if(cmd.mesh->getIndexCount() > 0) {
            glDrawElements(GL_TRIANGLES, cmd.mesh->getIndexCount(), cmd.mesh->getIndexType(), nullptr);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, cmd.mesh->getVertexCount());
        }

        m_lastCommandLastFrame = cmd;
    }

    RenderCommand Renderer::m_lastCommandLastFrame;

    RenderCommand& Renderer::getLastCommandLastFrame() {
        return m_lastCommandLastFrame;
    }

}