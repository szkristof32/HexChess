#include "pch.h"
#include "Engine.h"

namespace ChessEngine {

	Engine::Engine()
	{
		m_Window = std::make_unique<Window>(1280, 720, "HexChess");

		m_Running = true;
	}

	Engine::~Engine()
	{
	}

	void Engine::Run()
	{
		while (m_Running)
		{
			m_Window->Update();

			m_Running = !m_Window->ShouldClose();
		}
	}

}
