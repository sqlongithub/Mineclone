#pragma once
#include <vector>
#include <tuple>
#include <algorithm>
#include "Entity.h"
#include "ArchetypeBase.h"

namespace Mineclone {
    template<typename... Components>
    class Archetype : public ArchetypeBase {  // Make sure this inherits from ArchetypeBase
    public:
        void add(Entity e, Components... comps) {
            m_entities.push_back(e);
            m_components.emplace_back(std::move(comps)...);
        }

        bool remove(Entity e) override {
            auto it = std::find(m_entities.begin(), m_entities.end(), e);
            if (it == m_entities.end()) return false;

            size_t index = std::distance(m_entities.begin(), it);

            // swap with last and pop
            size_t last = m_entities.size() - 1;
            if (index != last) {
                m_entities[index] = m_entities[last];
                m_components[index] = std::move(m_components[last]);
            }

            m_entities.pop_back();
            m_components.pop_back();
            return true;
        }

        template<typename Func>
        void forEach(Func&& func) const {
            for (size_t i = 0; i < m_entities.size(); ++i) {
                std::apply([&](const auto&... comps) {
                    func(m_entities[i], const_cast<Components&>(comps)...);
                }, m_components[i]);
            }
        }

        [[nodiscard]] bool hasComponentType(std::type_index tid) const override {
            return ((tid == typeid(Components)) || ...);
        }

        [[nodiscard]] bool contains(Entity e) const override {
            return std::find(m_entities.cbegin(), m_entities.cend(), e) != m_entities.cend();
        }

        template<typename T>
        T& get(Entity e) {
            static_assert((std::is_same_v<T, Components> || ...), "Component not in archetype");
            auto it = std::find(m_entities.begin(), m_entities.end(), e);
            if (it == m_entities.end()) throw std::runtime_error("Entity not in archetype");

            size_t index = std::distance(m_entities.begin(), it);
            return std::get<T>(m_components[index]);
        }
    protected:
        void* getComponentImpl(Entity e, std::type_index tid) override {
            auto it = std::find(m_entities.begin(), m_entities.end(), e);
            if (it == m_entities.end()) {
                throw std::runtime_error("Entity not in archetype");
            }

            size_t index = std::distance(m_entities.begin(), it);
            return getComponentByType(m_components[index], tid);
        }

        [[nodiscard]] const void* getComponentImpl(Entity e, std::type_index tid) const override {
            auto it = std::find(m_entities.begin(), m_entities.end(), e);
            if (it == m_entities.end()) {
                throw std::runtime_error("Entity not in archetype");
            }

            size_t index = std::distance(m_entities.begin(), it);
            return getComponentByType(m_components[index], tid);
        }
    private:
        std::vector<Entity> m_entities;
        std::vector<std::tuple<Components...>> m_components;

        template<typename Tuple>
        void* getComponentByType(Tuple& tuple, std::type_index tid) {
            void* result = nullptr;
            std::apply([&](auto&... args) {
                (void)((tid == std::type_index(typeid(args)) && (result = &args, true)) || ...);
            }, tuple);

            if (!result) {
                throw std::runtime_error("Component type not found in archetype");
            }
            return result;
        }

        template<typename Tuple>
        const void* getComponentByType(const Tuple& tuple, std::type_index tid) const {
            const void* result = nullptr;
            std::apply([&](const auto&... args) {
                ((tid == std::type_index(typeid(args)) && (result = &args, true)) || ...);
            }, tuple);

            if (!result) {
                throw std::runtime_error("Component type not found in archetype");
            }
            return result;
        }
    };


}
