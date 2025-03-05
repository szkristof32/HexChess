#include "pch.h"
#include "Engine.h"

#include "ChessEngine/Core/Window.h"
#include "ChessEngine/Core/Client.h"
#include "ChessEngine/Core/ImGuiManager.h"

#include "ChessEngine/Rendering/Renderer.h"

namespace ChessEngine {

	Engine::Engine(Client* client)
	{
		m_Window = std::make_unique<Window>(1280, 720, "HexChess");
		m_Renderer = std::make_shared<Renderer>(m_Window->GetWindowHandle());
		m_ImGuiManager = std::make_unique<ImGuiManager>(m_Window->GetWindowHandle(), m_Renderer->GetRendererContext(), m_Renderer->GetRendererBackend());

		m_Window->RegisterSizeListener(std::bind(&Engine::OnResize, this, std::placeholders::_1, std::placeholders::_2));

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
			m_Running = !m_Window->ShouldClose();

			float frameTime = m_Window->GetTime();
			float delta = frameTime - m_LastFrameTime;
			m_LastFrameTime = frameTime;

			bool success = m_Renderer->BeginFrame();
			if (!success)
				continue;
			m_ImGuiManager->BeginFrame();

			if (m_Client)
				m_Client->OnUpdate(delta);

			m_ImGuiManager->EndFrame();
			m_Renderer->EndFrame();
		}
	}

	void Engine::OnResize(uint32_t width, uint32_t height)
	{
		m_Renderer->OnResize(width, height);

		if (m_Client)
			m_Client->OnResize(width, height);
	}

}
