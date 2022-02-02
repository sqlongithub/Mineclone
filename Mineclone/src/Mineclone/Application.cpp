#include <string>
#include <iostream>
#include <thread>

#include <glad/glad.h>
#include <glm/gtx/color_space.hpp>

#include "Application.h"
#include "OpenGL/Shader.h"
#include "OpenGL/VertexBuffer.h"
#include "OpenGL/IndexBuffer.h"
#include "Debug/Assert.h"

namespace Mineclone {

	Application::Application(const std::string& title, const int width, const int height)
		: m_window(title, width, height), m_logger("Application") {
		m_logger.info("Application launching");

		m_window.setVSync(true);

		/*
		m_window.setMainLoopCallback([this](GLFWwindow* window) {
			mainLoop(window);
		});
		*/
		
		m_window.setKeyCallback([this](const int key, const int scancode, const int action, const int mods) {
			onKey(key, scancode, action, mods);
		});


		std::thread fpsLogger([this]() { printFPS(); });
		m_window.run();
		run();

	}

	void Application::run() {

		GLFWwindow* window = m_window.getWindow();

		int hue = 0;
		float saturation = 0.8;
		float vibrance = 0.6;

		OpenGL::Shader shader = OpenGL::Shader("assets/shaders/BasicGradient.glsl");

		// vertices * data per vertex
		float vertices[4 * 3] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f,
		};

		unsigned int indices[2 * 3] = {
			0, 1, 2,
			2, 3, 0
		};

		unsigned int vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		OpenGL::VertexBuffer vertexBuffer(vertices, sizeof(vertices));

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

		OpenGL::IndexBuffer indexBuffer(indices, sizeof(indices) / sizeof(unsigned int));
		indexBuffer.bind();

		while(m_window.running()) {
			glClearColor(0.234f, 0.210f, 0.168f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glm::vec4 rgb = glm::vec4(glm::rgbColor(glm::vec3(hue, 0.5f, 0.8f)), 1.0f);
			if(hue == 360)
				hue = 0;
			else
				hue++;

			shader.setUniformFloat4("u_color", rgb);
			glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);

			double currentFrame = glfwGetTime();
			m_deltaTime = currentFrame - m_lastFrame;
			m_lastFrame = currentFrame;

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	void Application::printFPS()
	{
		using namespace std::literals::chrono_literals;
		std::this_thread::sleep_for(1s);
		while(m_window.running()) {
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
