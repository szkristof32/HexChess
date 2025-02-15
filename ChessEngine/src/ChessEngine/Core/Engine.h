#pragma once

#include "ChessEngine/Core/Window.h"

namespace ChessEngine {

	class Engine
	{
	public:
		Engine();
		~Engine();

		void Run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = false;
	};

}
