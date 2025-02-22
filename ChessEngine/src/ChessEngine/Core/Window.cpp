#include "pch.h"
#include "Window.h"

#include <GLFW/glfw3.h>

namespace ChessEngine {

	Window::Window(uint32_t width, uint32_t height, std::string_view title)
	{
		int result = glfwInit();
		glfwSetTime(0.0);

		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		GLFWwindow* window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

		const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwSetWindowPos(window, (vidmode->width - width) / 2, (vidmode->height - height) / 2);

		glfwShowWindow(window);
		m_WindowHandle = window;
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_WindowHandle);
		glfwTerminate();
	}

	void Window::Update() const
	{
		glfwPollEvents();
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_WindowHandle);
	}

	float Window::GetTime() const
	{
		return (float)glfwGetTime();
	}

}
