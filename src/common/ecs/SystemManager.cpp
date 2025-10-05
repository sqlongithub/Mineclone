#include "SystemManager.h"
#include "Registry.h"

namespace Mineclone {
    void SystemManager::updateAll(Registry& registry, float dt) {
        for (auto& sys : m_systems) {
            sys->update(registry, dt);
        }
    }

    void SystemManager::drawAll(Renderer& renderer, Registry& registry, float dt) {
        for (auto& sys : m_systems) {
            sys->draw(renderer, registry, dt);
        }
    }


    void SystemManager::clear() {
        m_systems.clear();
    }
}
