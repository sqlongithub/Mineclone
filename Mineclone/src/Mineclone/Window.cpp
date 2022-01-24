#include <string>

#include <GLFW/glfw3.h>

#include "Window.h"
#include <iostream>

namespace Mineclone {

	Window::Window(std::string title, int width, int height)
		: m_windowData(WindowData { nullptr, title, width, height, false }) {

		init();
		createWindow(title, width, height);
	}

	Window::Window()
		: m_windowData(WindowData {}) {

		init();
		createWindow(m_windowData.m_title, m_windowData.m_width, m_windowData.m_height);
	}

	void Window::init() const {
		// Initialize GLFW
		if (!glfwInit()) {
			
		}
	}

	void Window::createWindow(const std::string& title, int width, int height) {

	}

	void Window::close(const std::string& reason) const {
		glfwDestroyWindow(m_windowData.m_window);
		std::cout << "Window was closed: " << reason << "\n";
	}

	Window::~Window() {
		
	}

}    