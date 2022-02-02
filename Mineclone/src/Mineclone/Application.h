#pragma once 

#include "Debug/Log.h"
#include "OpenGL/Shader.h"
#include "Window.h"

#include <GLFW/glfw3.h>

namespace Mineclone {
	class Application {
	public:
		Application(const std::string& title, const int width, const int height);
		~Application();
	private:
		void mainLoop(GLFWwindow* window);
		void printFPS();
		void onKey(int key, int scancode, int action, int mods);
		void run();

		double m_deltaTime = 0.0;
		double m_lastFrame = 0.0;

		bool m_logFPS = false;

		Window m_window;
		Log m_logger;
	};
}