#pragma once

#include <ChessEngine/Core/Client.h>

#include "Loading/ModelRepository.h"

#include "Board/Board.h"
#include "Board/Piece.h"

#include "Board/BoardRenderer.h"
#include "Board/PieceRenderer.h"

#include "CameraController.h"

namespace HexChess {

	class ChessGame : public ChessEngine::Client
	{
	public:
		virtual void OnInit() override;

		virtual void OnUpdate(float deltaInSeconds) override;
		virtual void OnResize(uint32_t width, uint32_t height) override;
	private:
		std::shared_ptr<ChessEngine::Renderer> m_Renderer;
		std::shared_ptr<ModelRepository> m_ModelRepository;

		std::unique_ptr<Board> m_Board;
		
		std::unique_ptr<BoardRenderer> m_BoardRenderer;
		std::unique_ptr<PieceRenderer> m_PieceRenderer;
		
		std::unique_ptr<CameraController> m_CameraController;
	};

}
