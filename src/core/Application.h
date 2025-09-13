#pragma once

#include <string>
#include "Window.h"
#include "../game/Game.h"
#include "Dispatcher.h"

namespace Mineclone {

    class Application {
    public:
        Application(const std::string& title);
        void run();
    private:
        Dispatcher m_dispatcher;
        Window m_window;
        Renderer m_renderer;
        Registry m_registry;
        SystemManager m_systemManager;
        Game m_game;
    };

}
