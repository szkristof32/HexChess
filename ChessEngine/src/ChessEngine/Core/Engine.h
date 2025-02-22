#pragma once

namespace ChessEngine {

	class Window;
	class Renderer;
	class Client;

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
		std::unique_ptr<Window> m_Window;
		std::shared_ptr<Renderer> m_Renderer;

		std::unique_ptr<Client, NoDeleteter<Client>> m_Client;
		
		float m_LastFrameTime = 0.0f;
		bool m_Running = false;
	};

}
