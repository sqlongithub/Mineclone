#pragma once

#include "ISystem.h"
#include "Window.h"

namespace Mineclone {
    class SystemManager {
    public:
        SystemManager() = default;
        SystemManager(const SystemManager&) = delete;
        SystemManager& operator=(const SystemManager&) = delete;

        template<typename T, typename... Args>
        T& addSystem(Args&&... args) {
            static_assert(std::is_base_of<ISystem, T>::value, "System must inherit from ISystem");
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
        void drawAll(Registry &registry, float dt);
        void clear();

    private:

        std::vector<std::unique_ptr<ISystem>> m_systems;


    };
}