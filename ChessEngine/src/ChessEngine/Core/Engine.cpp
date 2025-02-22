#include "pch.h"
#include "Engine.h"

#include "ChessEngine/Core/Window.h"
#include "ChessEngine/Core/Client.h"

#include "ChessEngine/Rendering/Renderer.h"

namespace ChessEngine {

	Engine::Engine(Client* client)
	{
		m_Window = std::make_unique<Window>(1280, 720, "HexChess");
		m_Renderer = std::make_shared<Renderer>(m_Window->GetWindowHandle());

		if (client)
		{
			m_Client = std::unique_ptr<Client, NoDeleteter<Client>>(client);
			m_Client->GetRenderer = [&]() { return m_Renderer; };

			m_Client->OnInit();
		}

		m_Running = true;
	}

	Engine::~Engine()
	{
		if (m_Client)
			m_Client->OnShutdown();

		m_Renderer->WaitIdle();
	}

	void Engine::Run()
	{
		while (m_Running)
		{
			m_Window->Update();

			float frameTime = m_Window->GetTime();
			float delta = frameTime - m_LastFrameTime;
			m_LastFrameTime = frameTime;

			m_Renderer->BeginFrame();

			if (m_Client)
				m_Client->OnUpdate(delta);

			m_Renderer->EndFrame();

			m_Running = !m_Window->ShouldClose();
		}
	}

}
