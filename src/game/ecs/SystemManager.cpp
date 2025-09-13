#include "SystemManager.h"

namespace Mineclone {
    void SystemManager::updateAll(Registry& registry, float dt) {
        for (auto& sys : m_systems) {
            sys->update(registry, dt);
        }
    }

    void SystemManager::drawAll(Registry& registry, float dt) {
        for (auto& sys : m_systems) {
            sys->draw(registry, dt);
        }
    }


    void SystemManager::clear() {
        m_systems.clear();
    }
}
