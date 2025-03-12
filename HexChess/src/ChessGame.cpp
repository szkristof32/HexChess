#include <pch.h>
#include "ChessGame.h"

namespace HexChess {

	namespace BoardUtils {

		static glm::vec3 GetPiecePosition(uint32_t file, uint32_t rank, const BoardConfig& boardConfig)
		{
			float size = boardConfig.OuterSize;
			float hexWidth = 2.0f * size;
			float hexHeight = glm::sqrt(3.0f) * size;

			float distanceFromCenterX = 0.0f;
			float distanceFromCenterY = -2.0f;
			uint32_t fileOffset = 0;

			if (rank <= 5)
			{
				// These are the ranks with 11 cells
				distanceFromCenterX = 5.0f - file;
				distanceFromCenterY = 2.5f - rank;
				fileOffset = file <= 5 ? file : 10 - file;
			}
			else
			{
				// The rest
				uint32_t maxTileIndex = 20 - 2 * rank;

				distanceFromCenterX = (maxTileIndex / 2.0f) - file;
				distanceFromCenterY = -0.5f * rank;
				fileOffset = file <= (maxTileIndex / 2) ? file : maxTileIndex - file;
			}

			float xPosition = -distanceFromCenterX * size * 1.5f;
			float yPosition = (distanceFromCenterY + fileOffset / 2.0f) * hexHeight;

			return { xPosition, 0.0f, yPosition };
		}

	}

	void ChessGame::OnInit()
	{
		m_Renderer = GetRenderer();
		m_ModelRepository = std::make_shared<ModelRepository>(m_Renderer);

		m_Board = std::make_unique<Board>(m_Renderer, m_ModelRepository);

		m_BoardRenderer = std::make_unique<BoardRenderer>(m_Renderer);
		m_PieceRenderer = std::make_unique<PieceRenderer>(m_Renderer);
		
		m_CameraController = std::make_unique<CameraController>(GetInput());
	}

	void ChessGame::OnUpdate(float deltaInSeconds)
	{
		m_CameraController->Update(deltaInSeconds);

		m_Board->OnUpdate();

		m_BoardRenderer->BeginFrame(m_CameraController->GetProjectionMatrix(), m_CameraController->GetViewMatrix());
		m_BoardRenderer->RenderBoard(*m_Board);
		m_BoardRenderer->EndFrame();

		m_PieceRenderer->BeginFrame(m_CameraController->GetProjectionMatrix(), m_CameraController->GetViewMatrix());
		const auto& boardConfig = m_Board->GetConfig();
		const auto& pieces = m_Board->GetPieces();
		for (const auto& piece : pieces)
		{
			m_PieceRenderer->RenderPiece(piece, BoardUtils::GetPiecePosition(piece.GetFile(), piece.GetRank(), boardConfig));
		}
		m_PieceRenderer->EndFrame();
	}

	void ChessGame::OnResize(uint32_t width, uint32_t height)
	{
		m_CameraController->OnResize(width, height);
	}

}
