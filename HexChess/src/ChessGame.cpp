#include <pch.h>
#include "ChessGame.h"

namespace HexChess {

	void ChessGame::OnInit()
	{
		m_Renderer = GetRenderer();

		m_CameraController = std::make_unique<CameraController>(GetInput());
		m_Board = std::make_unique<Board>(m_Renderer);
	}

	void ChessGame::OnUpdate(float deltaInSeconds)
	{
		m_CameraController->Update(deltaInSeconds);

		m_Board->OnUpdate(m_CameraController->GetProjectionMatrix(), m_CameraController->GetViewMatrix());
	}

	void ChessGame::OnResize(uint32_t width, uint32_t height)
	{
		m_CameraController->OnResize(width, height);
	}

}
