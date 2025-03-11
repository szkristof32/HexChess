#include <pch.h>
#include "ChessGame.h"

namespace HexChess {

	void ChessGame::OnInit()
	{
		m_Renderer = GetRenderer();
		m_ModelRepository = std::make_unique<ModelRepository>(m_Renderer);

		m_CameraController = std::make_unique<CameraController>(GetInput());
		m_Board = std::make_unique<Board>(m_Renderer);
		m_PieceRenderer = std::make_unique<PieceRenderer>(m_Renderer);
	}

	void ChessGame::OnUpdate(float deltaInSeconds)
	{
		static Piece testPiece(PieceType::White | PieceType::Knight, *m_ModelRepository);

		m_CameraController->Update(deltaInSeconds);

		m_Board->OnUpdate(m_CameraController->GetProjectionMatrix(), m_CameraController->GetViewMatrix());

		m_PieceRenderer->BeginFrame(m_CameraController->GetProjectionMatrix(), m_CameraController->GetViewMatrix());
		m_PieceRenderer->RenderPiece(testPiece);
		m_PieceRenderer->EndFrame();
	}

	void ChessGame::OnResize(uint32_t width, uint32_t height)
	{
		m_CameraController->OnResize(width, height);
	}

}
