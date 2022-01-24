#pragma once
#include <string>

#include <GLFW/glfw3.h>

namespace Mineclone {
	class Window {
	public:

		Window(std::string title, int width, int height);
		Window();
		void init() const;
		~Window();

		struct WindowData {
			GLFWwindow* m_window;
			std::string m_title = "Mineclone";
			int m_width = 800;
			int m_height = 600;

			// Perhaps this should be stored in the Application class, since Window doesn't do any rendering
			bool m_vsync = false;
		};
	private:
		void createWindow(const std::string& title, int width, int height);
		void close(const std::string& reason) const;

		WindowData m_windowData;
	};
}