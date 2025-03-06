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
		glfwSetWindowUserPointer(window, this);

		const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwSetWindowPos(window, (vidmode->width - width) / 2, (vidmode->height - height) / 2);

		glfwSetFramebufferSizeCallback(window, [](GLFWwindow* windowPtr, int width, int height)
		{
			Window& window = *(Window*)glfwGetWindowUserPointer(windowPtr);

			for (const auto& listener : window.m_SizeListeners)
				listener((uint32_t)width, (uint32_t)height);
		});
		glfwSetKeyCallback(window, [](GLFWwindow* windowPtr, int key, int scancode, int action, int mods)
		{
			Window& window = *(Window*)glfwGetWindowUserPointer(windowPtr);

			switch (action)
			{
				case GLFW_PRESS:
					for (const auto& listener : window.m_KeyPressListeners)
						listener(key);
					break;
				case GLFW_RELEASE:
					for (const auto& listener : window.m_KeyReleaseListeners)
						listener(key);
					break;
			}
		});
		glfwSetMouseButtonCallback(window, [](GLFWwindow* windowPtr, int button, int action, int mods)
		{
			Window& window = *(Window*)glfwGetWindowUserPointer(windowPtr);

			switch (action)
			{
				case GLFW_PRESS:
					for (const auto& listener : window.m_ButtonPressListeners)
						listener(button);
					break;
				case GLFW_RELEASE:
					for (const auto& listener : window.m_ButtonReleaseListeners)
						listener(button);
					break;
			}
		});
		glfwSetCursorPosCallback(window, [](GLFWwindow* windowPtr, double xPos, double yPos)
		{
			Window& window = *(Window*)glfwGetWindowUserPointer(windowPtr);

			for (const auto& listener : window.m_MouseListeners)
				listener((float)xPos, (float)yPos);
		});
		glfwSetScrollCallback(window, [](GLFWwindow* windowPtr, double xOffset, double yOffset)
		{
			Window& window = *(Window*)glfwGetWindowUserPointer(windowPtr);

			for (const auto& listener : window.m_ScrollListeners)
				listener((float)yOffset);
		});

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
