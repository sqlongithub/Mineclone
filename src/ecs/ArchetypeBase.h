#pragma once

#include "Entity.h"
#include <typeindex>

namespace Mineclone {
    class ArchetypeBase {
    public:
        virtual ~ArchetypeBase() = default;

        virtual bool remove(Entity e) = 0;

        template<typename Component>
        [[nodiscard]] bool hasComponentType() const {
            return hasComponentType(std::type_index(typeid(Component)));
        }

        template<typename... Components>
        [[nodiscard]] bool hasAllComponents() const {
            return (hasComponentType<Components>() && ...);
        }

        [[nodiscard]] virtual bool hasComponentType(std::type_index tid) const = 0;

        [[nodiscard]] virtual bool contains(Entity e) const = 0;

        template<typename Component>
        Component& getComponent(Entity e) {
            void* ptr = getComponentImpl(e, std::type_index(typeid(Component)));
            return *static_cast<Component*>(ptr);
        }

        template<typename Component>
        const Component& getComponent(Entity e) const {
            const void* ptr = getComponentImpl(e, std::type_index(typeid(Component)));
            return *static_cast<const Component*>(ptr);
        }

    protected:
        virtual void* getComponentImpl(Entity e, std::type_index tid) = 0;
        virtual const void* getComponentImpl(Entity e, std::type_index tid) const = 0;
    };

}
