#include "Game.h"

#include <glm/gtx/string_cast.hpp>

#include "imgui.h"
#include "common/ecs/components/Transform.h"
#include "common/ecs/components/Velocity.h"
#include "common/physics/components/Collider.h"
#include "common/physics/components/MoveIntent.h"
#include "common/physics/systems/CollisionSystem.h"
#include "common/physics/systems/PhysicsMovementSystem.h"
#include "common/physics/systems/PlayerAbilitiesSystem.h"
#include "common/player/components/Player.h"
#include "common/world/systems/ChunkLoadingSystem.h"
#include "input/components/InputState.h"
#include "input/systems/CameraControlSystem.h"
#include "input/systems/DebugSystem.h"
#include "physics/systems/FreeCameraMovementSystem.h"
#include "physics/systems/MoveIntentSystem.h"
#include "rendering/components/Camera.h"
#include "rendering/components/CameraController.h"
#include "rendering/systems/ChunkRenderSystem.h"
#include "rendering/systems/RenderSystem.h"

namespace Mineclone {

    Game::Game(Registry& registry, SystemManager& systemManager, Window& window)
            : m_window(window),
              m_systemManager(systemManager),
              m_registry(registry),
              m_world(m_blockRegistry, m_biomeRegistry, 69420),
              m_blockRegistry(m_assetManager.blockTextures())
    {
        m_player = m_entityManager.create();
        m_registry.addEntity(m_player, Transform {
                {0, 80, 0}
        }, MoveIntent{}, Velocity{}, Camera{}, CameraController{}, Collider::player(), PlayerIdentity{0, true}, PlayerState { .gameMode = GameMode::NOCLIP });

        const auto& camT = registry.get<Transform>(m_player);
        auto& cam = registry.get<Camera>(m_player);
        std::cout << "initial Camera pos = " << glm::to_string(camT.position) << std::endl;
        initAssets();

#ifdef DEBUG
        printf("=== ACTUAL VERTEX STRUCT LAYOUT ===\n");
        printf("position offset: %zu\n", offsetof(BlockVertex, position));
        printf("faceIndex offset: %zu\n", offsetof(BlockVertex, faceIndex));
        printf("uv offset: %zu\n", offsetof(BlockVertex, uv));
        printf("textureIndex offset: %zu\n", offsetof(BlockVertex, textureIndex));
        printf("tintIndex offset: %zu\n", offsetof(BlockVertex, tintIndex));
        printf("sizeof(BlockVertex): %zu\n", sizeof(BlockVertex));
#endif
    }

    void Game::initAssets() {
        auto shader = m_assetManager.shaders().create("texture", "resources/shaders/texture.vert", "resources/shaders/texture.frag");
        auto materialHandle = m_assetManager.materials().create("texture", shader, &m_assetManager.blockTextures());
        auto material = m_assetManager.materials().get(materialHandle);



        if(!m_assetManager.blockTextures().addTextures("resources/textures/blocks")) {
            std::cerr << "Failed to load textures!" << std::endl;
        }

        m_blockRegistry.loadFromDirectory("resources/blocks");
        m_biomeRegistry.loadFromDirectory("resources/biomes");

        material->parameters.setVec3Array("uBiomeTints", m_biomeRegistry.getTints());

    }

    void Game::initSystems() {

        m_systemManager.addSystem<DebugSystem>();
        m_systemManager.addSystem<ChunkRenderSystem>(m_world, m_assetManager, m_blockRegistry, m_biomeRegistry);

        m_systemManager.addSystem<RenderSystem>(m_assetManager);
        m_systemManager.getSystem<RenderSystem>().setActiveCamera(m_player);

        m_registry.getSingleton<InputState>().isCursorCaptured = true;

        m_systemManager.addSystem<ChunkLoadingSystem>(m_world, 20);
        m_systemManager.addSystem<CameraControlSystem>();
        m_systemManager.addSystem<MoveIntentSystem>();
        m_systemManager.addSystem<PlayerAbilitiesSystem>();
        m_systemManager.addSystem<PhysicsMovementSystem>(m_world);
        m_systemManager.addSystem<FreeCameraMovementSystem>(m_world);
        m_systemManager.addSystem<CollisionSystem>(m_world, m_blockRegistry);
    }

    void Game::update(float dt) {
        m_systemManager.updateAll(m_registry, dt);
    }

    void Game::draw(Renderer &renderer, float dt) const {
        auto& camT = m_registry.get<Transform>(m_player);
        auto& cam = m_registry.get<Camera>(m_player);
        auto& camController = m_registry.get<CameraController>(m_player);

        auto windowSize = m_window.getSize();
        RenderCamera rc = cam.toRenderCamera(
            camT,
            camController.fov,
            camController.eyeHeight,
            windowSize.first,
            windowSize.second
        );
        renderer.beginFrame(rc);

        m_systemManager.drawAll(renderer, m_registry, dt);

        ImGui::Begin("World");
        const auto& chunks = m_world.getLoadedChunks();
        ImGui::Text("Loaded chunks: %zu", chunks.size());

        if (ImGui::TreeNode("Looking at")) {
            auto hit = m_world.raycast(camT.position, camT.getForwardVector());
            if(!hit.has_value()) {
                ImGui::Text("Nothing");
            } else {
                const auto& block = hit->block;
                auto faceIdx = static_cast<int>(hit->face);

                ImGui::Text("World pos: (%d, %d, %d)", hit->blockPos.x, hit->blockPos.y, hit->blockPos.z);
                ImGui::Text("Id: %d (%s)", block.id, m_blockRegistry.getMetadata(block.id)->name.data());

                auto biomeId = m_world.getBiome(hit->blockPos);
                ImGui::Text("Biome: %d (%s)", biomeId, m_biomeRegistry.getBiomeName(biomeId));

                const auto& model = m_blockRegistry.getModel(block.id);
                if (ImGui::TreeNode("Elements")) {
                    for(size_t i = 0; i < model->elements.size(); i++) {
                        const auto& element = model->elements[i];
                        if (ImGui::TreeNode(std::to_string(i).data())) {
                            ImGui::Text("from: (%d, %d, %d)", (int)element.from[0], (int)element.from[1], (int)element.from[2]);
                            ImGui::Text("to: (%d, %d, %d)", (int)element.to[0], (int)element.to[1], (int)element.to[2]);
                            if(ImGui::TreeNode("Faces")) {
                                for(const auto& face : element.faces) {
                                    if(ImGui::TreeNode(face.first.data())) {
                                        const auto faceDef = face.second;
                                        ImGui::Text("uv: (%d, %d) -> (%d, %d)", (int)faceDef.uv[0], (int)faceDef.uv[1], (int)faceDef.uv[2], (int)faceDef.uv[3]);
                                        ImGui::Text("texture: %s (index %d)", faceDef.texture.data(), m_assetManager.blockTextures().getTextureIndex(faceDef.texture));
                                        if(faceDef.cullFace.has_value()) {
                                            ImGui::Text("cullface: %s", faceDef.cullFace->data());
                                        } else {
                                            ImGui::Text("cullface: none");
                                        }
                                        auto tint = m_biomeRegistry.getTints()[faceDef.tintIndex];
                                        ImGui::Text("tintindex: %d -> (%d, %d, %d)", faceDef.tintIndex, tint.x, tint.y, tint.z);
                                        ImGui::TreePop();
                                    }
                                }
                                ImGui::TreePop();
                            }
                            ImGui::TreePop();
                        }
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
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
        if(ImGui::TreeNode("Biomes")) {
            for(int i = 0; i < m_biomeRegistry.getBiomeCount(); i++) {
                if(ImGui::TreeNode(m_biomeRegistry.getBiomeName(i)->get().data())) {
                    if(ImGui::TreeNode("Metadata")) {
                        ImGui::Text("Name: %s", m_biomeRegistry.getBiomeName(i)->get().data());
                        ImGui::TreePop();
                    }
                    auto tint = m_biomeRegistry.getTints()[i];
                    ImGui::Text("Tint: (%f, %f, %f)", tint.x, tint.y, tint.z);
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
        if(ImGui::TreeNode("Blocks")) {
            for(size_t i = 1; i < m_blockRegistry.getBlockCount(); i++) {
                const auto& meta = m_blockRegistry.getMetadata(i);
                if(ImGui::TreeNode(meta->name.data())) {
                    ImGui::Text("ID: %d", i);
                    if(ImGui::TreeNode("Model")) {
                        const auto& model = m_blockRegistry.getModel(i);
                        if (ImGui::TreeNode("Elements")) {
                            for(size_t j = 0; j < model->elements.size(); j++) {
                                const auto& element = model->elements[j];
                                if (ImGui::TreeNode(std::to_string(j).data())) {
                                    ImGui::Text("from: (%f, %f, %f)", (int)element.from[0], (int)element.from[1], (int)element.from[2]);
                                    ImGui::Text("to: (%f, %f, %f)", (int)element.to[0], (int)element.to[1], (int)element.to[2]);
                                    if(ImGui::TreeNode("Faces")) {
                                        for(const auto& face : element.faces) {
                                            if(ImGui::TreeNode(face.first.data())) {
                                                const auto faceDef = face.second;
                                                ImGui::Text("uv: (%f, %f) -> (%f, %f)", (int)faceDef.uv[0], (int)faceDef.uv[1], (int)faceDef.uv[2], (int)faceDef.uv[3]);
                                                ImGui::Text("texture: %s (index %d)", faceDef.texture.data(), m_assetManager.blockTextures().getTextureIndex(faceDef.texture));
                                                if(faceDef.cullFace.has_value()) {
                                                    ImGui::Text("cullface: %s", faceDef.cullFace->data());
                                                } else {
                                                    ImGui::Text("cullface: none");
                                                }
                                                auto tint = m_biomeRegistry.getTints()[faceDef.tintIndex];
                                                ImGui::Text("tintindex: %d -> (%f, %f, %f)", faceDef.tintIndex, tint.x, tint.y, tint.z);
                                                ImGui::Text("layer: %d", faceDef.layer);
                                                ImGui::TreePop();
                                            }
                                        }
                                        ImGui::TreePop();
                                    }
                                    ImGui::TreePop();
                                }
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
