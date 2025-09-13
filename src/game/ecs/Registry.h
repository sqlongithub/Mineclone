#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <iostream>
#include "Entity.h"
#include "Archetype.h"

namespace Mineclone {

    class Registry {
    public:
        Registry() = default;
        Registry(const Registry&) = delete;
        Registry& operator=(const Registry&) = delete;

        template<typename... Components>
        void addEntity(Entity e, Components... comps) {
            ensureArchetype<Components...>().add(e, std::move(comps)...);
        }

        void removeEntity(Entity e) {
            for (auto& [tid, archetype] : m_archetypes) {
                if (archetype->contains(e)) {
                    archetype->remove(e);
                }
            }
        }

        template<typename... Components, typename Func>
        void view(Func&& func) const {
            for (auto& [tid, archetype] : m_archetypes) {
                // Check if this archetype contains ALL the requested components
                if (archetype->hasAllComponents<Components...>()) {
                    static_cast<const Archetype<Components...>*>(archetype.get())
                            ->forEach(std::forward<Func>(func));
                }
            }
        }


        template<typename Component>
        Component& get(Entity e) {
            // Find the archetype that contains Component
            for (auto& [tid, archetype] : m_archetypes) {
                if (archetype->hasComponentType<Component>() && archetype->contains(e)) {
                    return archetype->template getComponent<Component>(e);
                }
            }
            throw std::runtime_error("Entity " + std::to_string(e) + " does not have the requested component");
        }

        template<typename Component>
        const Component& get(Entity e) const {
            for (auto& [tid, archetype] : m_archetypes) {
                if (archetype->hasComponentType<Component>() && archetype->contains(e)) {
                    return archetype->template getComponent<Component>(e);
                }
            }
            throw std::runtime_error("Entity does not have the requested component");
        }

        template<typename T, typename... Args>
        void setSingleton(Args&&... args) {
            static_assert(std::is_move_constructible_v<T>, "Singleton component must be move constructible");
            auto tid = std::type_index(typeid(T));
            m_singletons[tid] = std::make_unique<SingletonWrapper<T>>(T{std::forward<Args>(args)...});
        }

        template<typename T>
        T& getSingleton() {
            auto tid = std::type_index(typeid(T));
            auto it = m_singletons.find(tid);

            if (it == m_singletons.end()) {
                // Auto-create with default constructor if it doesn't exist
                setSingleton<T>();
                it = m_singletons.find(tid);
            }

            return static_cast<SingletonWrapper<T>*>(it->second.get())->component;
        }

        template<typename T>
        const T& getSingleton() const {
            auto tid = std::type_index(typeid(T));
            auto it = m_singletons.find(tid);

            if (it == m_singletons.end()) {
                throw std::runtime_error("Singleton component not found");
            }

            return static_cast<const SingletonWrapper<T>*>(it->second.get())->component;
        }

        template<typename T>
        bool hasSingleton() const {
            auto tid = std::type_index(typeid(T));
            return m_singletons.find(tid) != m_singletons.end();
        }

        template<typename T>
        void removeSingleton() {
            auto tid = std::type_index(typeid(T));
            m_singletons.erase(tid);
        }



    private:
        std::unordered_map<std::type_index, std::unique_ptr<ArchetypeBase>> m_archetypes;

        struct SingletonWrapperBase {
            virtual ~SingletonWrapperBase() = default;
        };

        template<typename T>
        struct SingletonWrapper : SingletonWrapperBase {
            T component;

            template<typename... Args>
            explicit SingletonWrapper(Args&&... args) : component(std::forward<Args>(args)...) {}
        };

        std::unordered_map<std::type_index, std::unique_ptr<SingletonWrapperBase>> m_singletons;

        template<typename... Components>
        Archetype<Components...>& ensureArchetype() {
            auto tid = std::type_index(typeid(Archetype<Components...>));
            auto it = m_archetypes.find(tid);

            if (it == m_archetypes.end()) {
                auto archetype = std::make_unique<Archetype<Components...>>();
                auto* ptr = archetype.get();
                m_archetypes.emplace(tid, std::move(archetype));
                return *ptr;
            }
            return *static_cast<Archetype<Components...>*>(it->second.get());
        }
    };

}
