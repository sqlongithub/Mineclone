#pragma once 
#include <chrono>

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
		void printFPS();
		void onKey(int key, int scancode, int action, int mods);

		double m_deltaTime;
		double m_lastFrame;
		std::chrono::time_point<std::chrono::steady_clock> m_startTime;
		bool m_logFPS = false;
		bool m_running = true;

		Window m_window;
		Log m_logger;
	};
}