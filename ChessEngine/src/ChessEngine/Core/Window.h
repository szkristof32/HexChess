#pragma once

struct GLFWwindow;

namespace ChessEngine {

	class Window
	{
	public:
		using SizeListener = std::function<void(uint32_t, uint32_t)>;
	public:
		Window(uint32_t width, uint32_t height, std::string_view title);
		~Window();

		void Update() const;
		void RegisterSizeListener(const SizeListener& listener) { m_SizeListeners.emplace_back(listener); }

		bool ShouldClose() const;
		float GetTime() const;

		GLFWwindow* GetWindowHandle() const { return m_WindowHandle; }
	private:
		GLFWwindow* m_WindowHandle;

		std::vector<SizeListener> m_SizeListeners;
	};

}
