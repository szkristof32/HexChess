#include "pch.h"
#include "Engine.h"

#include "ChessEngine/Core/Window.h"
#include "ChessEngine/Rendering/Renderer.h"

namespace ChessEngine {

	Engine::Engine()
	{
		m_Window = std::make_unique<Window>(1280, 720, "HexChess");
		m_Renderer = std::make_unique<Renderer>();

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

			m_Renderer->BeginFrame();
			m_Renderer->EndFrame();

			m_Running = !m_Window->ShouldClose();
		}
	}

}
