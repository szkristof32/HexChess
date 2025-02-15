#pragma once

struct GLFWwindow;

namespace ChessEngine {

	class Window
	{
	public:
		Window(uint32_t width, uint32_t height, std::string_view title);
		~Window();

		void Update() const;

		bool ShouldClose() const;
	private:
		GLFWwindow* m_WindowHandle;
	};

}
