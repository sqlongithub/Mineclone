#include "Game.h"
#include "gpu/Vertex.h"
#include "mesh/Mesh.h"
#include "glm/detail/type_mat4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtx/string_cast.hpp"
#include "RenderSystem.h"
#include "Renderable.h"
#include "RenderLayer.h"
#include "ImGuiRenderSystem.h"
#include "DebugSystem.h"
#include "CursorSystem.h"
#include "ChunkRenderSystem.h"
#include "imgui.h"
#include "FreeCameraMovementSystem.h"
#include "InputState.h"

namespace Mineclone {

    Game::Game(Registry& registry, SystemManager& systemManager, Dispatcher& dispatcher)
            : m_assetManager(),
              m_entityManager(),
              m_systemManager(systemManager),
              m_registry(registry),
              m_dispatcher(dispatcher),
              m_world(m_blockRegistry, 69420),
              m_blockRegistry(m_assetManager.blockTextures())
    {
        m_camera = m_entityManager.create();
        m_registry.addEntity(m_camera, Transform {
                {0, 50, 0}
        }, Camera{});

        auto& camT = registry.get<Transform>(m_camera);
        auto& cam = registry.get<Camera>(m_camera);
        std::cout << "initial Camera pos = " << glm::to_string(camT.position) << std::endl;

        initAssets();

        m_world.loadChunksAroundPosition(camT.position, 1);
    }

    void Game::initAssets() {
        auto shader = m_assetManager.shaders().create("texture", "resources/shaders/texture.vert", "resources/shaders/texture.frag");
        auto material = m_assetManager.materials().create("texture", shader, &m_assetManager.blockTextures());

        if(!m_assetManager.blockTextures().addTextures("resources/textures/blocks")) {
            std::cerr << "Failed to load textures!" << std::endl;
        }

        m_blockRegistry.loadFromDirectory("resources/blocks");

    }

    void Game::initSystems() {
        m_systemManager.addSystem<FreeCameraMovementSystem>(m_world);
        m_systemManager.addSystem<DebugSystem>(m_dispatcher);
        m_systemManager.addSystem<ChunkRenderSystem>(m_world, m_assetManager, m_blockRegistry);

        m_systemManager.addSystem<RenderSystem>(m_assetManager);
        m_systemManager.getSystem<RenderSystem>().setActiveCamera(m_camera);

        m_registry.getSingleton<InputState>().isCursorCaptured = true;
    }

    void Game::update(float dt) {
        m_systemManager.updateAll(m_registry, dt);
    }

    void Game::draw(Renderer &renderer, float dt) const {
        auto& camT = m_registry.get<Transform>(m_camera);
        auto& cam = m_registry.get<Camera>(m_camera);
        renderer.beginFrame(cam.toRenderCamera(camT));

        m_systemManager.drawAll(renderer, m_registry, dt);

        ImGui::Begin("World");
        const auto& chunks = m_world.getLoadedChunks();
        ImGui::Text("Loaded chunks: %zu", chunks.size());

        for (auto& [pos, chunk] : chunks) {
            int nonAirCount = 0;

            for (auto slice: chunk.getNonEmptySlices()) {
                for (uint8_t y = 0; y < slice.getHeight(); y++)
                    for (uint8_t z = 0; z < CHUNK_DEPTH; z++)
                        for (uint8_t x = 0; x < CHUNK_WIDTH; x++)
                            if (slice.getBlock(x, y, z).id != AIR_BLOCK_ID)
                                nonAirCount++;
            }

            if (ImGui::TreeNode((void *) (intptr_t) &chunk, "Chunk (%d,%d)", pos.x, pos.z)) {
                ImGui::Text("Non-air blocks: %d", nonAirCount);

                // Optionally, show first few block types
                int count = 0;
                for (auto slice: chunk.getNonEmptySlices()) {
                    for (uint8_t y = 0; y < slice.getHeight(); y++)
                        for (uint8_t z = 0; z < CHUNK_DEPTH; z++)
                            for (uint8_t x = 0; x < CHUNK_WIDTH; x++) {
                                const Block &b = slice.getBlock(x, y, z);
                                if (b.id != AIR_BLOCK_ID && count < 10) {
                                    ImGui::Text("Block at local (%d,%d,%d): type %d", x, y, z, b.id);
                                    count++;
                                }
                            }
                }

                ImGui::TreePop();
            }
        }
        ImGui::End();

        ImGui::Begin("Renderer");
        ImGui::Text("Last command last frame:");
        RenderCommand& cmd = Mineclone::Renderer::getLastCommandLastFrame();
        if(ImGui::TreeNode("Mesh")) {
            ImGui::Text("Vertices: %d", cmd.mesh->getVertexCount());
            ImGui::Text("Indices: %d", cmd.mesh->getIndexCount());
            ImGui::Text("VAO index: %d", cmd.mesh->getVAO().getId());
            ImGui::Text("Index type: %d", cmd.mesh->getIndexType());
            ImGui::TreePop();
        }
        if(ImGui::TreeNode("Material")) {
            auto& mat = *m_assetManager.materials().get(cmd.material);
            auto& shader = *m_assetManager.shaders().get(mat.shader);
            ImGui::Text("Shader ID: %d", shader.getID());
            ImGui::Text("Valid mat: %d", mat.isValid());
            ImGui::TreePop();
        }
        ImGui::End();

        ImGui::Begin("Assets");
        if(ImGui::TreeNode("Blocks")) {
            for(size_t i = 1; i < m_blockRegistry.getBlockCount(); i++) {
                const auto& meta = m_blockRegistry.getMetadata(i);
                if(ImGui::TreeNode(meta->name.data())) {
                    ImGui::Text("ID: %d", i);
                    if(ImGui::TreeNode("Model")) {
                        const auto& model = m_blockRegistry.getModel(i);
                        if(ImGui::TreeNode("Faces")) {
                            for(int j = 0; j < 6; j++) {
                                const auto face = static_cast<CubeFace>(j);
                                int texIndex = m_blockRegistry.getTextureIndex(i, face);
                                ImGui::Text("%s: Index %d: %s", getCubeFaceName(face).data(), texIndex, m_assetManager.blockTextures().getTextureName(texIndex).data());
                            }
                            ImGui::TreePop();
                        }
                        ImGui::TreePop();
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
        ImGui::End();


        renderer.flushQueue(m_assetManager);
    }


}
