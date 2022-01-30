#include <string>
#include <iostream>
#include <thread>

#include <glad/glad.h>

#include "Application.h"
#include "OpenGL/Shader.h"
#include "Debug/Assert.h"

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

	Application::Application(const std::string& title, const int width, const int height)
		: m_window(title, width, height), m_logger("Application") {
		m_logger.info("Application launching");

		{
			// triangles * vertices per triangle * data per triangle
			float vertices[2 * 3 * 6] = {
				-0.5f, -0.5f, 0.0f,		1.0f, 0.5f, 0.0f, // 0
				0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.5f, // 1
				0.5f, 0.5f, 0.0f,		0.5f, 0.0f, 1.0f, // 2
				-0.5f, 0.5f, 0.0f,		1.0f, 0.0f, 0.5f  // 3
			};

			unsigned int indices[2 * 3] = {
				0, 1, 2,
				2, 3, 0
			};

			unsigned int buffer;
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

			unsigned int ibo;
			glGenBuffers(1, &ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			OpenGL::Shader shader("assets/shaders/BasicGradient.glsl");

			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(OpenGLMessageCallback, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);

			m_window.setVSync(true);
			m_window.setMainLoopCallback([this](GLFWwindow* window) {
				mainLoop(window);
			});
			
			m_window.setKeyCallback([this](const int key, const int scancode, const int action, const int mods) {
				onKey(key, scancode, action, mods);
			});

			std::thread fpsLogger([this]() { printFPS(); });
			m_window.run(); // Keep the application running
			m_running = false;
		}

	}

	void Application::mainLoop(GLFWwindow* window) {

		glClearColor(0.234f, 0.210f, 0.168f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 2*3, GL_FLOAT, nullptr);

		double currentFrame = glfwGetTime();
		m_deltaTime = currentFrame - m_lastFrame;
		m_lastFrame = currentFrame;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	void Application::printFPS()
	{
		using namespace std::literals::chrono_literals;
		std::this_thread::sleep_for(1s);
		while(m_running) {
			if(!m_logFPS) {
				return;
			}
			m_logger.info("FPS: " + std::to_string(1 / m_deltaTime) + " (" + std::to_string(m_deltaTime * 1000) + " ms)");
			std::this_thread::sleep_for(1s);
		}
	}

	void Application::onKey(int key, int scancode, int action, int mods)
	{
		if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			m_window.close("User exited");
		}
	}

	Application::~Application()
	{
		glfwTerminate();
	};
}
