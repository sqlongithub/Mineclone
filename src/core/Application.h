#pragma once

#include <string>
#include "Window.h"
#include "../game/Game.h"

namespace Mineclone {

    class Application {
    public:
        Application(const std::string& title);
        void run();
    private:
        Window m_window;
        Renderer m_renderer;
        Game m_game;
    };

}
