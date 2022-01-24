#include <string>

#include "Application.h"

namespace Mineclone {
	Application::Application(const std::string& title, int width, int height)
		: m_window(Window(title, width, height)), m_logger("Application") {
		m_logger.info("Application launching");
		m_logger.debug("Application info:\n" +
			std::string(" - Title: ") + title + "\n" +
			std::string(" - Size: ") + std::to_string(width) + "x" + std::to_string(height) + "\n");
	}

	Application::~Application() {	
	}
}
