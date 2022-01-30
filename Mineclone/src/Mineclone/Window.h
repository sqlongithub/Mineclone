#pragma once
#include <functional>
#include <string>

#include <GLFW/glfw3.h>

#include "Debug/Log.h"

namespace Mineclone {
	class Window {
	public:

		Window(const std::string& title, const int width, const int height);
		Window(const Window& window);
		Window();
		~Window();

		void close(const std::string& reason) const;
		static void close(GLFWwindow* window, const std::string& reason);

		struct WindowData {
			GLFWwindow* window;
			std::string title = "Window";
			int width = 800;
			int height = 600;

			Log logger;

			// TODO: find optimal way to store and call callbacks
			std::function<void(int width, int height)> resizeCallback = [](int width, int height) {};
			std::function<void()> closeCallback = []() {};
			std::function<void(int key, int scancode, int action, int mods)> keyCallback = [](int key, int scancode, int action, int mods) {};
			std::function<void(unsigned int key)> charCallback = [](unsigned int key) {};
			std::function<void(int button, int action, int mods)> mouseButtonCallback = [](int button, int action, int mods) {};
			std::function<void(double x, double y)> scrollCallback = [](double x, double y) {};
			std::function<void(double x, double y)> mouseMoveCallback = [](double x, double y) {};
		};

		void run() const;

		void setVSync(const bool enabled);
		void setResizeCallback(const std::function<void(int width, int height)>& resizeCallback);
		void setCloseCallback(const std::function<void()>& closeCallback);
		void setKeyCallback(const std::function<void(int key, int scancode, int action, int mods)>& keyCallback);
		void setCharCallback(const std::function<void(unsigned int key)>& charCallback);
		void setMouseButtonCallback(const std::function<void(int button, int action, int mods)>& mouseButtonCallback);
		void setScrollCallback(const std::function<void(double x, double y)>& scrollCallback);
		void setMouseMoveCallback(const std::function<void(double x, double y)>& mouseMoveCallback);
		void setMainLoopCallback(const std::function<void(GLFWwindow*)>& mainLoopCallback);

	private:
		void createWindow(const std::string& title, int width, int height);
		void init() const;
		void setCallbacks() const;

		// TODO: log an error if no main loop callback
		std::function<void(GLFWwindow*)> m_mainLoopCallback;
		WindowData m_windowData;
		Log& m_logger;
		bool m_vsync = true;
	};
}