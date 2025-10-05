#include "CollisionSystem.h"
#include <glm/glm.hpp>
#include <cmath>
#include <algorithm>

namespace Mineclone {

    static constexpr float EPS = 1e-4f;

    CollisionSystem::CollisionSystem(World& world, BlockRegistry& blockRegistry)
        : m_world(world), m_blockRegistry(blockRegistry) {}

    float CollisionSystem::computeAllowedMovement(const glm::vec3& pos, const Collider& collider,
                                                  float movement, int axis) {
        if (std::abs(movement) < EPS) return 0.0f;

        const glm::vec3 minPos = pos + collider.aabb.min;
        const glm::vec3 maxPos = pos + collider.aabb.max;

        glm::vec3 testPos = pos;
        testPos[axis] += movement;
        const glm::vec3 entityMin = testPos + collider.aabb.min;
        const glm::vec3 entityMax = testPos + collider.aabb.max;

        float allowed = movement;

        int minX = static_cast<int>(std::floor(std::min(minPos.x, entityMin.x)));
        int maxX = static_cast<int>(std::floor(std::max(maxPos.x, entityMax.x)));
        int minY = static_cast<int>(std::floor(std::min(minPos.y, entityMin.y)));
        int maxY = static_cast<int>(std::floor(std::max(maxPos.y, entityMax.y)));
        int minZ = static_cast<int>(std::floor(std::min(minPos.z, entityMin.z)));
        int maxZ = static_cast<int>(std::floor(std::max(maxPos.z, entityMax.z)));

        for (int x = minX; x <= maxX; ++x) {
            for (int y = minY; y <= maxY; ++y) {
                for (int z = minZ; z <= maxZ; ++z) {
                    int chunkX = x >> 4;
                    int chunkZ = z >> 4;
                    int localX = x & 15;
                    int localZ = z & 15;
                    if (localX < 0) { localX += 16; chunkX--; }
                    if (localZ < 0) { localZ += 16; chunkZ--; }

                    ChunkPos cp{ chunkX, chunkZ };
                    Chunk& chunk = m_world.getChunk(cp);
                    const Block* block = chunk.getBlock(localX, y, localZ);
                    if (!block) continue;
                    auto meta = m_blockRegistry.getMetadata(block->id);
                    if (!meta || !meta->solid) continue;

                    glm::vec3 blockMin(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
                    glm::vec3 blockMax = blockMin + glm::vec3(1.0f);

                    bool overlap = true;
                    for (int i = 0; i < 3; ++i) {
                        if (i == axis) continue;
                        if (maxPos[i] <= blockMin[i] || minPos[i] >= blockMax[i]) {
                            overlap = false;
                            break;
                        }
                    }
                    if (!overlap) continue;

                    if (movement > 0.0f) allowed = std::min(allowed, blockMin[axis] - maxPos[axis]);
                    else allowed = std::max(allowed, blockMax[axis] - minPos[axis]);
                }
            }
        }

        return allowed;
    }

    bool CollisionSystem::tryStepUp(Transform& transform, const Collider& collider,
                                     float reqX, float reqZ, float stepHeight) {
        glm::vec3 savedPos = transform.position;

        float allowedY = computeAllowedMovement(transform.position, collider, stepHeight, 1);
        if (allowedY < EPS) return false;

        transform.position.y += allowedY;

        float allowedX = computeAllowedMovement(transform.position, collider, reqX, 0);
        float allowedZ = computeAllowedMovement(transform.position, collider, reqZ, 2);

        if (std::abs(allowedX - reqX) > EPS || std::abs(allowedZ - reqZ) > EPS) {
            transform.position = savedPos;
            return false;
        }

        transform.position.x += allowedX;
        transform.position.z += allowedZ;

        float drop = computeAllowedMovement(transform.position, collider, -allowedY, 1);
        transform.position.y += drop;

        return true;
    }

    void CollisionSystem::processEntity(Entity e, Transform& transform, Velocity& velocity,
                                        Collider& collider, PlayerState* pstate,
                                        const PhysicsProperties& props, float dt) {
        if (!collider.isDynamic) return;
        if (pstate && !pstate->hasCollision()) return;

        glm::vec3 requested = velocity.velocity * dt;

        float allowedY = computeAllowedMovement(transform.position, collider, requested.y, 1);
        transform.position.y += allowedY;

        bool grounded = (requested.y < 0.0f && std::abs(allowedY - requested.y) > EPS);
        if (grounded) velocity.velocity.y = 0.0f;

        if (pstate) {
            if (grounded) {
                pstate->isGrounded = true;
                pstate->lastGrounded = 0.0f;
            } else {
                pstate->isGrounded = false;
                pstate->lastGrounded += dt;
            }
        }

        float allowedX = computeAllowedMovement(transform.position, collider, requested.x, 0);
        float allowedZ = computeAllowedMovement(transform.position, collider, requested.z, 2);

        bool xBlocked = std::abs(allowedX - requested.x) > EPS;
        bool zBlocked = std::abs(allowedZ - requested.z) > EPS;

        if ((xBlocked || zBlocked) && grounded &&
            (std::abs(requested.x) > EPS || std::abs(requested.z) > EPS)) {
            if (!tryStepUp(transform, collider, requested.x, requested.z, props.stepHeight)) {
                transform.position.x += allowedX;
                transform.position.z += allowedZ;
                if (xBlocked) velocity.velocity.x = 0.0f;
                if (zBlocked) velocity.velocity.z = 0.0f;
            }
        } else {
            transform.position.x += allowedX;
            transform.position.z += allowedZ;
            if (xBlocked) velocity.velocity.x = 0.0f;
            if (zBlocked) velocity.velocity.z = 0.0f;
        }
    }

    void CollisionSystem::update(Registry& registry, float dt) {
        const auto& props = registry.getSingleton<PhysicsProperties>();

        registry.view<Transform, Velocity, Collider, PlayerState>(
            [&](Entity e, Transform& t, Velocity& v, Collider& c, PlayerState& ps) {
                processEntity(e, t, v, c, &ps, props, dt);
            });

        registry.view<Transform, Velocity, Collider>(
            [&](Entity e, Transform& t, Velocity& v, Collider& c) {
                if (registry.hasComponent<PlayerState>(e)) return;
                processEntity(e, t, v, c, nullptr, props, dt);
            });
    }

}
