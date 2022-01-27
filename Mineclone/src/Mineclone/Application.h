#pragma once 

#include <GLFW/glfw3.h>

#include "Log.h"
#include "Window.h"

namespace Mineclone {
	class Application {
	public:
		Application(const std::string& title, const int width, const int height);
		~Application();
	private:
		void mainLoop(GLFWwindow* window);

		Window m_window;
		Log m_logger;
	};
}