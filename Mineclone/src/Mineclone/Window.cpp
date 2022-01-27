#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window.h"

// Inspiration taken from the Hazel Game engine by TheCherno:
// https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Platform/Windows/WindowsWindow.cpp
namespace Mineclone {

	Window::Window(std::string title, int width, int height)
		: m_windowData(WindowData { nullptr, title, width, height, Log("Window") }), 
		  m_logger(m_windowData.logger) {

		init();
		createWindow(title, width, height);
	}

	Window::Window()
		: m_windowData(WindowData { nullptr, "Window", 800, 600, Log("Window")}),
		  m_logger(m_windowData.logger) {

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

		setCallbacks();
	}

	void Window::setCallbacks() {
		glfwSetWindowSizeCallback(m_windowData.window, [](GLFWwindow* window, int width, int height) {
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			windowData.width = width;
			windowData.height = height;

			windowData.resizeCallback(width, height);
		});

		glfwSetWindowCloseCallback(m_windowData.window, [](GLFWwindow* window) {
			const WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			close(windowData.window, "Window closed by user");
			windowData.closeCallback();
		});

		glfwSetKeyCallback(m_windowData.window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			const WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			windowData.keyCallback(key, scancode, action, mods);
		});

		glfwSetCharCallback(m_windowData.window, [](GLFWwindow* window, unsigned int key) {
			const WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			windowData.charCallback(key);
		});

		glfwSetMouseButtonCallback(m_windowData.window, [](GLFWwindow* window, int button, int action, int mods) {
			const WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			windowData.mouseButtonCallback(button, action, mods);
		});

		glfwSetScrollCallback(m_windowData.window, [](GLFWwindow* window, double x, double y) {
			const WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			windowData.scrollCallback(x, y);
		});

		glfwSetCursorPosCallback(m_windowData.window, [](GLFWwindow* window, double x, double y) {
			const WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			windowData.mouseMoveCallback(x, y);
		});
	}

	void Window::setMainLoopCallback(std::function<void(GLFWwindow*)> mainLoopCallback) {
		m_mainLoopCallback = mainLoopCallback;
	}

	void Window::setResizeCallback(void(*resizeCallback)(int width, int height)) {
		m_windowData.resizeCallback = resizeCallback;
	}

	void Window::setCloseCallback(void(*closeCallback)()) {
		m_windowData.closeCallback = closeCallback;
	}

	void Window::setKeyCallback(void(*keyCallback)(int key, int scancode, int action, int mods)) {
		m_windowData.keyCallback = keyCallback;
	}

	void Window::setCharCallback(void(*charCallback)(unsigned int key)) {
		m_windowData.charCallback = charCallback;
	}

	void Window::setMouseButtonCallback(void(*mouseButtonCallback)(int button, int action, int mods)) {
		m_windowData.mouseButtonCallback = mouseButtonCallback;
	}

	void Window::setScrollCallback(void(*scrollCallback)(double x, double y)) {
		m_windowData.scrollCallback = scrollCallback;
	}

	void Window::setMouseMoveCallback(void(*mouseMoveCallback)(double x, double y)) {
		m_windowData.mouseMoveCallback = mouseMoveCallback;
	}

	void Window::run() const
	{
		while(!glfwWindowShouldClose(m_windowData.window)) {
			m_mainLoopCallback(m_windowData.window);
		}
	}

	void Window::close(const std::string& reason) const {
		glfwDestroyWindow(m_windowData.window);
		m_logger.info("Window closed: " + reason);
	}

	void Window::close(GLFWwindow* window, const std::string& reason) {
		// TODO: maybe we should log the title of the window?
		WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
		glfwDestroyWindow(window);
		windowData.logger.info("Window closed: " + reason);
	}

	void Window::setVSync(bool enabled) {
		m_vsync = enabled;
		glfwSwapInterval((int) enabled);
	}

	Window::~Window() {
		
	}

}    