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
	}

	void ChessGame::OnResize(uint32_t width, uint32_t height)
	{
		m_Board->OnResize(width, height);
	}

}
