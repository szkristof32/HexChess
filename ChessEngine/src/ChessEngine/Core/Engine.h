#pragma once

namespace ChessEngine {

	class Window;
	class Renderer;

	class Engine
	{
	public:
		Engine();
		~Engine();

		void Run();
	private:
		std::unique_ptr<Window> m_Window;
		std::unique_ptr<Renderer> m_Renderer;
		
		bool m_Running = false;
	};

}
