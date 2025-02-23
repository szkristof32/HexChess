#pragma once

#include <ChessEngine/Core/Client.h>

#include "Board.h"

namespace HexChess {

	class ChessGame : public ChessEngine::Client
	{
	public:
		virtual void OnInit() override;

		virtual void OnUpdate(float deltaInSeconds) override;
	private:
		std::shared_ptr<ChessEngine::Renderer> m_Renderer;

		std::unique_ptr<Board> m_Board;
	};

}
