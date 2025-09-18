#pragma once

#include "System.h"
#include "Window.h"
#include "Registry.h"
#include "Renderer.h"

namespace Mineclone {
    class SystemManager {
    public:
        SystemManager() = default;
        SystemManager(const SystemManager&) = delete;
        SystemManager& operator=(const SystemManager&) = delete;

        template<typename T, typename... Args>
        T& addSystem(Args&&... args) {
            static_assert(std::is_base_of<System, T>::value, "System must inherit from System");
            auto sys = std::make_unique<T>(std::forward<Args>(args)...);
            T& ref = *sys;
            m_systems.push_back(std::move(sys));
            return ref;
        }

        template<typename T>
        T& getSystem() {
            for (auto& sys : m_systems) {
                if (auto casted = dynamic_cast<T*>(sys.get()))
                    return *casted;
            }
            throw std::runtime_error("System not found");
        }

        void updateAll(Registry& registry, float dt);
        void drawAll(Renderer& renderer, Registry &registry, float dt);
        void clear();

    private:
        std::vector<std::unique_ptr<System>> m_systems;
    };
}