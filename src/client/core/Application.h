#pragma once
#include <string>

#include "Window.h"
#include "client/Game.h"

#include "client/input/InputContextManager.h"
#include "client/rendering/Renderer.h"
#include "common/ecs/Registry.h"
#include "common/ecs/SystemManager.h"

namespace Mineclone {

    class Application {
    public:
        Application(const std::string& title);
        void run();
    private:
        Window m_window;
        Renderer m_renderer;
        Registry m_registry;
        SystemManager m_systemManager;
        InputContextManager m_contextManager;
        Game m_game;
    };

}
