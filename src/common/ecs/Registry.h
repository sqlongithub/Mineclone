#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <iostream>
#include <ranges>

#include "Entity.h"
#include "Archetype.h"
#include <string>

namespace Mineclone {

    class Registry {
    public:
        Registry() = default;
        Registry(const Registry&) = delete;
        Registry& operator=(const Registry&) = delete;

        template<typename... Components>
        void addEntity(Entity e, Components&&... comps) {
            ensureArchetype<std::decay_t<Components>...>().add(e, std::forward<Components>(comps)...);
        }


        void removeEntity(Entity e) {
            for (const auto& archetype: m_archetypes | std::views::values) {
                if (archetype->contains(e)) {
                    archetype->remove(e);
                }
            }
        }

        template<typename... Components, typename Func>
        void view(Func&& func) {
            for (const auto& archetype : m_archetypes | std::views::values) {
                if (!archetype->hasAllComponents<Components...>())
                    continue;
                for (Entity e : archetype->entities()) {
                    func(e, archetype->template getComponent<Components>(e)...);
                }
            }
        }

        template<typename Component>
        Component& get(Entity e) {
            for (const auto& archetype: m_archetypes | std::views::values) {
                if (archetype->hasComponentType<Component>() && archetype->contains(e)) {
                    return archetype->template getComponent<Component>(e);
                }
            }
            throw std::runtime_error("Entity " + std::to_string(e) + " does not have the requested component");
        }

        template<typename Component>
        bool hasComponent(const Entity e) const {
            for (const auto& archetype: m_archetypes | std::views::values) {
                if (archetype->hasComponentType<Component>() && archetype->contains(e)) {
                    return true;
                }
            }
            return false;
        }

        template<typename Component>
        const Component& get(const Entity e) const {
            for (const auto &archetype: m_archetypes | std::views::values) {
                if (archetype->hasComponentType<Component>() && archetype->contains(e)) {
                    return archetype->template getComponent<Component>(e);
                }
            }
            throw std::runtime_error("Entity does not have the requested component");
        }

        template<typename T, typename... Args>
        void setSingleton(Args&&... args) {
            static_assert(std::is_move_constructible_v<T>, "Singleton component must be move constructible");
            static_assert(std::is_copy_assignable_v<T> || std::is_move_assignable_v<T>,
                         "Singleton component must be assignable");

            auto tid = std::type_index(typeid(T));

            if constexpr (sizeof...(Args) == 0) {
                m_singletons[tid] = std::make_unique<SingletonWrapper<T>>(T{});
            } else {
                m_singletons[tid] = std::make_unique<SingletonWrapper<T>>(T{std::forward<Args>(args)...});
            }
        }

        template<typename T>
        T& getSingleton() {
            auto tid = std::type_index(typeid(T));
            auto it = m_singletons.find(tid);

            if (it == m_singletons.end()) {
                setSingleton<T>();
                it = m_singletons.find(tid);
            }

            auto* wrapper = dynamic_cast<SingletonWrapper<T>*>(it->second.get());
            if (!wrapper) {
                throw std::runtime_error("Singleton type mismatch - requested type doesn't match stored type");
            }

            return wrapper->component;
        }

        template<typename T>
        const T& getSingleton() const {
            auto tid = std::type_index(typeid(T));
            auto it = m_singletons.find(tid);

            if (it == m_singletons.end()) {
                throw std::runtime_error("Singleton component not found");
            }

            const auto* wrapper = dynamic_cast<const SingletonWrapper<T>*>(it->second.get());
            if (!wrapper) {
                throw std::runtime_error("Singleton type mismatch - requested type doesn't match stored type");
            }

            return wrapper->component;
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
        struct SingletonWrapper final : SingletonWrapperBase {
            T component;

            explicit SingletonWrapper(T comp) : component(std::move(comp)) {}
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