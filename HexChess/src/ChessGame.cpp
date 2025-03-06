#include <pch.h>
#include "ChessGame.h"

namespace HexChess {

	void ChessGame::OnInit()
	{
		m_Renderer = GetRenderer();

		m_Board = std::make_unique<Board>(m_Renderer);
	}

	void ChessGame::OnUpdate(float deltaInSeconds)
	{
		m_Board->OnUpdate();

		std::cout << GetInput()->IsButtonDown(0) << " ";
		std::cout << GetInput()->IsKeyDown('A') << " ";
		std::cout << GetInput()->IsKeyPressed('R') << "\n";
	}

	void ChessGame::OnResize(uint32_t width, uint32_t height)
	{
		m_Board->OnResize(width, height);
	}

}
