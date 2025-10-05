#define GLM_ENABLE_EXPERIMENTAL

#include "glad/glad.h"
#include "Renderer.h"
#include "materials/Shader.h"
#include "mesh/Mesh.h"
#include "glm/gtx/string_cast.hpp"
#include <algorithm>

namespace Mineclone {

    Renderer::Renderer()
        : m_renderQueue(),
          m_frameUBO(sizeof(FrameData), 0) {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
     //   glEnable(GL_BLEND);
     //   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
        // opaque front to back
        auto& opaqueCmds = m_renderQueue.getCommands(Layer::TERRAIN);
        std::ranges::sort(opaqueCmds,
                          [this](const RenderCommand& a, const RenderCommand& b) {
                              if (a.material.index != b.material.index)
                                  return a.material.index < b.material.index;
                              return a.getDistanceToCamera(m_frameData.cameraPosition) <
                                     b.getDistanceToCamera(m_frameData.cameraPosition);
                          });
        for (auto& cmd : opaqueCmds) executeCommand(cmd, assets);

        // transparent back to front
        auto& transCmds = m_renderQueue.getCommands(Layer::TRANSPARENT);
        std::ranges::sort(transCmds,
                          [this](const RenderCommand& a, const RenderCommand& b) {
                              return a.getDistanceToCamera(m_frameData.cameraPosition) >
                                     b.getDistanceToCamera(m_frameData.cameraPosition);
                          });

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);

        for (auto& cmd : transCmds) executeCommand(cmd, assets);

        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);

        // ui
        for (const auto& uiCmds = m_renderQueue.getCommands(Layer::UI);
             auto& cmd : uiCmds) {
            executeCommand(cmd, assets);
        }

        m_renderQueue.clear();
    }

    void Renderer::submit(const RenderCommand& cmd) {
        m_renderQueue.add(cmd);
    }

    void Renderer::executeCommand(const RenderCommand& cmd, const AssetManager& assets) {
        if (!cmd.mesh || !cmd.material.isValid())
            return;

        const Material* material = assets.materials().get(cmd.material);
        if (!material || !material->isValid()) {
            std::cerr << "Invalid material" << std::endl;
            return;
        }

        const Shader* shader = assets.shaders().get(material->shader);
        if (!shader) {
            std::cerr << "Invalid shader" << std::endl;
            return;
        }

        shader->bind();
        material->textures->bind(0);
        shader->setUniformInt("uTextureArray", 0);
        shader->setUniformMat4("uModel", cmd.modelMatrix);
        material->parameters.applyToShader(*shader);

        const Mesh& mesh = *cmd.mesh;
        if (mesh.isEmpty()) return;

        mesh.getVAO().bind();

        if (cmd.submeshIndex < UINT32_MAX && cmd.submeshIndex < mesh.getSubmeshCount()) {
            const auto& submesh = mesh.getSubmesh(cmd.submeshIndex);


#ifdef DEBUG
            if (submesh.isEmpty) {
                std::cerr << "[Renderer] Submesh " << cmd.submeshIndex << " is marked empty." << std::endl;
            }
            if (submesh.indexCount == 0) {
                std::cerr << "[Renderer] Submesh " << cmd.submeshIndex << " has zero indices." << std::endl;
            }
            if (submesh.indexOffset + submesh.indexCount > mesh.getIndexCount()) {
                std::cerr << "[Renderer] Submesh " << cmd.submeshIndex
                          << " has invalid range: offset=" << submesh.indexOffset
                          << " count=" << submesh.indexCount
                          << " totalIndexCount=" << mesh.getIndexCount() << std::endl;
            }
#endif

            if (!submesh.isEmpty && submesh.indexCount > 0) {
                size_t indexSize = (mesh.getIndexType() == GL_UNSIGNED_SHORT) ? 2 : 4;
                glDrawElementsBaseVertex(
                    GL_TRIANGLES,
                    static_cast<GLsizei>(submesh.indexCount),
                    mesh.getIndexType(),
                    reinterpret_cast<void*>(submesh.indexOffset * indexSize),
                    submesh.baseVertex
                );
            }
        } else {
            if (mesh.getIndexCount() > 0) {
                glDrawElements(GL_TRIANGLES, mesh.getIndexCount(), mesh.getIndexType(), nullptr);
            }
        }

        mesh.getVAO().unbind();
        m_lastCommandLastFrame = cmd;
    }

    RenderCommand Renderer::m_lastCommandLastFrame;

    RenderCommand& Renderer::getLastCommandLastFrame() {
        return m_lastCommandLastFrame;
    }
}