#include <string>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Log.h"

// Inspiration taken from the Hazel Game engine by TheCherno:
// https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Platform/Windows/WindowsWindow.cpp
namespace Mineclone {

	Window::Window(std::string title, int width, int height)
		: m_windowData(WindowData { nullptr, title, width, height }), m_logger("Window") {

		init();
		createWindow(title, width, height);
	}

	Window::Window()
		: m_windowData(WindowData {}) {

		init();
		createWindow(m_windowData.title, m_windowData.width, m_windowData.height);
	}

	void Window::init() {
		// Initialize GLFW
		m_logger.debug("Initializing..");
		if(!glfwInit()) {
			m_logger.error("Failed to initialize GLFW");
			exit(-1);
		}
	}

	void Window::createWindow(const std::string& title, int width, int height) {
		m_logger.debug("Creating a window..");
		m_logger.debug("Window info:\n" +
			std::string("	- Title: ") + title + "\n" +
			std::string("	- Size: ") + std::to_string(width) + "x" + std::to_string(height) + "\n");

		m_windowData.window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_windowData.window);

		if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			m_logger.error("Failed to initialize GLAD");
			exit(-1);
		}

		glViewport(0, 0, width, height);

		setVSync(true);

		glfwSetWindowUserPointer(m_windowData.window, &m_windowData);

		glfwSetWindowSizeCallback(m_windowData.window, [](GLFWwindow* window, int width, int height) {
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			windowData.width = width;
			windowData.height = height;
			
			windowData.resizeCallback(width, height);
		});

		
	}

	void Window::setResizeCallback(std::function<void(int width, int height)> resizeCallback) {
		m_windowData.resizeCallback = resizeCallback;
	}

	void Window::setCloseCallback(std::function<void()> closeCallback) {
		m_windowData.closeCallback = closeCallback;
	}

	void Window::setKeyCallback(std::function<void(int key, int scancode, int action, int mods)> keyCallback) {
		m_windowData.keyCallback = keyCallback;
	}

	void Window::setCharCallback(std::function<void(unsigned int key)> charCallback) {
		m_windowData.charCallback = charCallback;
	}

	void Window::setMouseButtonCallback(std::function<void(int button, int action, int mods)> mouseClickCallback) {
		m_windowData.mouseClickCallback = mouseClickCallback;
	}

	void Window::setScrollCallback(std::function<void(double x, double y)> scrollCallback) {
		m_windowData.scrollCallback = scrollCallback;
	}

	void Window::setMouseMoveCallback(std::function<void(double x, double y)> mouseMoveCallback) {
		m_windowData.mouseMoveCallback = mouseMoveCallback;
	}

	void Window::close(const std::string& reason) {
		glfwDestroyWindow(m_windowData.window);
		m_logger.info("Window closed");
	}

	void Window::setVSync(bool enabled) {
		m_vsync = enabled;
		glfwSwapInterval((int) enabled);
	}

	Window::~Window() {
		
	}

}    