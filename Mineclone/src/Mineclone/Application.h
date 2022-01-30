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

		double m_deltaTime = 0.0;
		double m_lastFrame = 0.0;

		uint16_t hue = 1;
		float g = 0.4f;
		float b = 0.8f;
		float increment = 0.005f;

		bool m_logFPS = false;
		bool m_running = true;

		std::unique_ptr<OpenGL::Shader> m_shader;

		Window m_window;
		Log m_logger;
	};
}