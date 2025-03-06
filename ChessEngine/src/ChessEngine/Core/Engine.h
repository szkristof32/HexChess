#pragma once

#include "ChessEngine/Core/Input.h"

namespace ChessEngine {

	class Window;
	class Renderer;
	class Client;
	class ImGuiManager;

	template<typename T>
	struct NoDeleteter
	{
		inline void operator()(T* ptr) {}
	};

	class Engine
	{
	public:
		Engine(Client* client = nullptr);
		~Engine();

		void Run();
	private:
		void OnResize(uint32_t width, uint32_t height);
	private:
		std::unique_ptr<Window> m_Window;
		std::shared_ptr<Renderer> m_Renderer;
		std::unique_ptr<ImGuiManager> m_ImGuiManager;
		std::shared_ptr<Input> m_Input;

		std::unique_ptr<Client, NoDeleteter<Client>> m_Client;
		
		float m_LastFrameTime = 0.0f;
		bool m_Running = false;
	};

}
