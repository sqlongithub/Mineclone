#include <string>

#include "Application.h"

namespace Mineclone {
	Application::Application(const std::string& title, int width, int height)
		: m_window(Window(title, width, height)), m_logger("Application") {
		m_logger.info("Application launching");
	}

	Application::~Application() {	
	}
}
