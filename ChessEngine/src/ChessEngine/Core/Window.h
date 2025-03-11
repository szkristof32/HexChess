#pragma once

struct GLFWwindow;

namespace ChessEngine {

	class Window
	{
	public:
		using SizeListener = std::function<void(uint32_t, uint32_t)>;
		using KeyListener = std::function<void(uint32_t)>;
		using ButtonListener = std::function<void(uint32_t)>;
		using MouseListener = std::function<void(float, float)>;
		using ScrollListener = std::function<void(float)>;
	public:
		Window(uint32_t width, uint32_t height, std::string_view title);
		~Window();

		void OnInit() const;
		void Update() const;

		void RegisterSizeListener(const SizeListener& listener) { m_SizeListeners.emplace_back(listener); }
		void RegisterKeyPressListener(const KeyListener& listener) { m_KeyPressListeners.emplace_back(listener); }
		void RegisterKeyReleaseListener(const KeyListener& listener) { m_KeyReleaseListeners.emplace_back(listener); }
		void RegisterButtonPressListener(const ButtonListener& listener) { m_ButtonPressListeners.emplace_back(listener); }
		void RegisterButtonReleaseListener(const ButtonListener& listener) { m_ButtonReleaseListeners.emplace_back(listener); }
		void RegisterMouseListener(const MouseListener& listener) { m_MouseListeners.emplace_back(listener); }
		void RegisterScrollListener(const ScrollListener& listener) { m_ScrollListeners.emplace_back(listener); }

		bool ShouldClose() const;
		float GetTime() const;

		GLFWwindow* GetWindowHandle() const { return m_WindowHandle; }
	private:
		GLFWwindow* m_WindowHandle;

		std::vector<SizeListener> m_SizeListeners;
		std::vector<KeyListener> m_KeyPressListeners;
		std::vector<KeyListener> m_KeyReleaseListeners;
		std::vector<ButtonListener> m_ButtonPressListeners;
		std::vector<ButtonListener> m_ButtonReleaseListeners;
		std::vector<MouseListener> m_MouseListeners;
		std::vector<ScrollListener> m_ScrollListeners;
	};

}
