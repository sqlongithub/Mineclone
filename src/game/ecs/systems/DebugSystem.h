#pragma once

#include "ISystem.h"
#include "Dispatcher.h"

namespace Mineclone {
    class DebugSystem : public ISystem {
    public:
        DebugSystem(Dispatcher& dispatcher);
        void update(Mineclone::Registry &registry, float dt) override;
    private:
        Dispatcher& m_dispatcher;
    };

}
