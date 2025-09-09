#pragma once

#include <string>
#include "Window.h"

namespace Mineclone {

    class Application {
    public:
        Application(const std::string& title);
    private:
        Window m_window;
    };

}
