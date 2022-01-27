#include <string>

#include "Application.h"

namespace Mineclone {
	Application::Application(const std::string& title, const int width, const int height)
		: m_window(Window(title, width, height)), m_logger("Application") {
		m_logger.info("Application launching");
		m_window.setMainLoopCallback([this](GLFWwindow* window) { mainLoop(window); });
		m_window.run(); // Keep the application running
	}

	void Application::mainLoop(GLFWwindow* window) {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Application::~Application() {	
	}
}
