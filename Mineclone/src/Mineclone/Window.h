#pragma once
#include <functional>
#include <string>

#include <GLFW/glfw3.h>

#include "Log.h"

namespace Mineclone {
	class Window {
	public:

		Window(std::string title, int width, int height);
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

		void setVSync(bool enabled);
		void setResizeCallback(void(*resizeCallback)(int width, int height));
		void setCloseCallback(void(*closeCallback)());
		///	 key: the key pressed
		///  <param name="scancode">the location of the key</param>
	    ///  <param name="action">press, release or hold</param>
	    ///  <param name="mods"alt, shift, alt+gr, lctrl, rctrl</param>
		void setKeyCallback(void(*keyCallback)(int key, int scancode, int action, int mods));
		void setCharCallback(void(*charCallback)(unsigned int key));
		void setMouseButtonCallback(void(*mouseButtonCallback)(int button, int action, int mods));
		void setScrollCallback(void(*scrollCallback)(double x, double y));
		void setMouseMoveCallback(void(*mouseMoveCallback)(double x, double y));
		void setMainLoopCallback(std::function<void(GLFWwindow*)> mainLoopCallback);

	private:
		void createWindow(const std::string& title, int width, int height);
		void init();
		void setCallbacks();
		
		std::function<void(GLFWwindow*)> m_mainLoopCallback;
		Log& m_logger;
		WindowData m_windowData;
		bool m_vsync = true;
	};
}