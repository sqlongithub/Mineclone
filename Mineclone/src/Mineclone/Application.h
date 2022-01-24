#pragma once 
#include <iostream>

#include <GLFW/glfw3.h>

#include "Log.h"
#include "Window.h"

namespace Mineclone {
	class Application {
	public:
		Application(const std::string& title, int width, int height);
		~Application();
	private:
		Window m_window;
		Log m_logger;
	};
}