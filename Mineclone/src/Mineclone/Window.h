#pragma once
#include <string>
#include <functional>
#include <vector>

#include <GLFW/glfw3.h>

#include "Log.h"

namespace Mineclone {
	class Window {
	public:

		Window(std::string title, int width, int height);
		Window();
		~Window();

		struct WindowData {
			GLFWwindow* window;
			std::string title = "Mineclone";
			int width = 800;
			int height = 600;

			// TODO: find optimal way to store and call callbacks
			std::function<void(int width, int height)> resizeCallback;
			std::function<void()> closeCallback;
			std::function<void(int key, int scancode, int action, int mods)> keyCallback;
			std::function<void(unsigned int key)> charCallback;
			std::function<void(int button, int action, int mods)> mouseClickCallback;
			std::function<void(double x, double y)> scrollCallback;
			std::function<void(double x, double y)> mouseMoveCallback;
		};

		void setVSync(bool enabled);
		void setResizeCallback(std::function<void(int width, int height)> resizeCallback);
		void setCloseCallback(std::function<void()> closeCallback);
		//	key: the key pressed
		//  scancode: the location of the key
	    //  action: press, release or hold
	    //  mods: alt, shift, alt+gr, lctrl, rctrl
		void setKeyCallback(std::function<void(int key, int scancode, int action, int mods)> keyCallback);
		void setCharCallback(std::function<void(unsigned int key)> charCallback);
		void setMouseButtonCallback(std::function<void(int button, int action, int mods)> mouseClickCallback);
		void setScrollCallback(std::function<void(double x, double y)> scrollCallback);
		void setMouseMoveCallback(std::function<void(double x, double y)> mouseMoveCallback);

	private:
		void createWindow(const std::string& title, int width, int height);
		void close(const std::string& reason);
		void init();

		Log m_logger;
		WindowData m_windowData;
		bool m_vsync = true;
	};
}