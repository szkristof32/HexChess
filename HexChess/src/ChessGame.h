#pragma once

#include <ChessEngine/Core/Client.h>

#include "Board/Board.h"

namespace HexChess {

	class ChessGame : public ChessEngine::Client
	{
	public:
		virtual void OnInit() override;

		virtual void OnUpdate(float deltaInSeconds) override;
		virtual void OnResize(uint32_t width, uint32_t height) override;
	private:
		std::shared_ptr<ChessEngine::Renderer> m_Renderer;

		std::unique_ptr<Board> m_Board;
	};

}
