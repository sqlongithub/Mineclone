#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Debug/Assert.h"

// Inspiration taken from the Hazel Game engine by TheCherno:
// https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Platform/Windows/WindowsWindow.cpp
namespace Mineclone {

	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch(severity) {
		case GL_DEBUG_SEVERITY_HIGH:         Log::error("OpenGL", message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       Log::warn("OpenGL", message); return;
		case GL_DEBUG_SEVERITY_LOW:          Log::info("OpenGL", message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: Log::debug("OpenGL", message); return;
		}

		ASSERT_NOT_REACHED("Severity \"" + std::to_string(severity) + "\" unknown!");
	}

	Window::Window(const std::string& title, const int width, const int height)
		: m_mainLoopCallback([](GLFWwindow*) {}),
		m_windowData(WindowData{ nullptr, title, width, height, false, Log("Window") }),
		m_logger(m_windowData.logger) {

		init();
		createWindow(title, width, height);
	}

	Window::Window(const Window& window)
		: m_mainLoopCallback([](GLFWwindow*) {}),
		m_windowData(WindowData{ nullptr, window.m_windowData.title, window.m_windowData.width, window.m_windowData.height, m_windowData.running, window.m_windowData.logger }),
		m_logger(m_windowData.logger) {
		
	}

	Window::Window()
		: m_windowData(WindowData { nullptr, "Window", 800, 600, false, Log("Window")}),
		  m_logger(m_windowData.logger) {

		init();
		createWindow(m_windowData.title, m_windowData.width, m_windowData.height);
	}

	void Window::init() const {
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

		if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
			m_logger.error("Failed to initialize GLAD");
			exit(-1);
		}

		glViewport(0, 0, width, height);

		setVSync(true);

		glfwSetWindowUserPointer(m_windowData.window, &m_windowData);

		setCallbacks();

	}

	void Window::setCallbacks() const {

		glfwSetWindowSizeCallback(m_windowData.window, [](GLFWwindow* window, int width, int height) {
			WindowData& windowData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			windowData.width = width;
			windowData.height = height;

			glViewport(0, 0, width, height);
			windowData.resizeCallback(width, height);
		});

		glfwSetWindowCloseCallback(m_windowData.window, [](GLFWwindow* window) {
			const WindowData& windowData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			close(windowData.window, "Window closed by user");
			windowData.closeCallback();
		});

		glfwSetKeyCallback(m_windowData.window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			const WindowData& windowData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			windowData.keyCallback(key, scancode, action, mods);
		});

		glfwSetCharCallback(m_windowData.window, [](GLFWwindow* window, unsigned int key) {
			const WindowData& windowData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			windowData.charCallback(key);
		});

		glfwSetMouseButtonCallback(m_windowData.window, [](GLFWwindow* window, int button, int action, int mods) {
			const WindowData& windowData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			windowData.mouseButtonCallback(button, action, mods);
		});

		glfwSetScrollCallback(m_windowData.window, [](GLFWwindow* window, double x, double y) {
			const WindowData& windowData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			windowData.scrollCallback(x, y);
		});

		glfwSetCursorPosCallback(m_windowData.window, [](GLFWwindow* window, double x, double y) {
			const WindowData& windowData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			windowData.mouseMoveCallback(x, y);
		});

		// Enable logging
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
	}

	void Window::setMainLoopCallback(const std::function<void(GLFWwindow*)>& mainLoopCallback) {
		m_mainLoopCallback = mainLoopCallback;
	}

	void Window::setResizeCallback(const std::function<void(int width, int height)>& resizeCallback) {
		m_windowData.resizeCallback = resizeCallback;
	}

	void Window::setCloseCallback(const std::function<void()>& closeCallback) {
		m_windowData.closeCallback = closeCallback;
	}

	void Window::setKeyCallback(const std::function<void(int key, int scancode, int action, int mods)>& keyCallback) {
		m_windowData.keyCallback = keyCallback;
	}

	void Window::setCharCallback(const std::function<void(unsigned int key)>& charCallback) {
		m_windowData.charCallback = charCallback;
	}

	void Window::setMouseButtonCallback(const std::function<void(int button, int action, int mods)>& mouseButtonCallback) {
		m_windowData.mouseButtonCallback = mouseButtonCallback;
	}

	void Window::setScrollCallback(const std::function<void(double x, double y)>& scrollCallback) {
		m_windowData.scrollCallback = scrollCallback;
	}

	void Window::setMouseMoveCallback(const std::function<void(double x, double y)>& mouseMoveCallback) {
		m_windowData.mouseMoveCallback = mouseMoveCallback;
	}

	void Window::run() {
		/*while(!glfwWindowShouldClose(m_windowData.window)) {
			m_mainLoopCallback(m_windowData.window);
		}*/
		m_windowData.running = true;
	}

	void Window::close(const std::string& reason) {
		glfwSetWindowShouldClose(m_windowData.window, true);
		glfwDestroyWindow(m_windowData.window);
		glfwTerminate();
		m_windowData.running = false;
		m_logger.info("Window closed: " + reason);
	}

	void Window::close(GLFWwindow* window, const std::string& reason) {
		// TODO: maybe we should log the title of the window?
		WindowData& windowData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
		glfwSetWindowShouldClose(window, true);
		glfwDestroyWindow(window);
		windowData.running = false;
		windowData.logger.info("Window closed: " + reason);
	}

	void Window::setVSync(const bool enabled) {
		m_vsync = enabled;
		glfwSwapInterval((int) enabled);
	}

	Window::~Window() {
		m_windowData.running = false;
		glfwSetWindowShouldClose(m_windowData.window, true);
		glfwDestroyWindow(m_windowData.window);
		glfwTerminate();
	};

}    