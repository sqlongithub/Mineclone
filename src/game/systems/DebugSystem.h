#pragma once

#include "System.h"
#include "Dispatcher.h"
#include "Registry.h"

namespace Mineclone {
    class DebugSystem : public System {
    public:
        DebugSystem(Dispatcher& dispatcher);
        void update(Mineclone::Registry &registry, float dt) override;
    private:
        Dispatcher& m_dispatcher;
    };

}
