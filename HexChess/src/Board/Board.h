#pragma once

#include "Model.h"
#include "Board/Piece.h"
#include "Board/BoardGenerator.h"

#include <ChessEngine/Rendering/Renderer.h>
#include <ChessEngine/Rendering/Buffers.h>

#include <glm/glm.hpp>

namespace HexChess {

	class Board
	{
	public:
		static constexpr const char* DefaultFEN = "b/qbk/n1b1n/r5r/ppppppppp/11/5P5/4P1P4/3P1B1P3/2P2B2P2/1PRNQBKNRP1";
	public:
		Board(const std::shared_ptr<ChessEngine::Renderer>& renderer, const std::shared_ptr<ModelRepository>& modelRepository);
		~Board();

		void OnUpdate();

		const Model& GetModel() const { return m_Model; }
		const BoardConfig& GetConfig() const { return m_BoardConfig; }

		const std::vector<Piece>& GetPieces() const { return m_Pieces; }
	private:
		void GenerateBoard();
		void GeneratePieces(std::string_view fen = DefaultFEN);

		void RenderUI();
	private:
		std::shared_ptr<ChessEngine::Renderer> m_Renderer;

		BoardConfig m_BoardConfig;
		BoardGenerator m_Generator;

		std::vector<Piece> m_Pieces;
		std::shared_ptr<ModelRepository> m_ModelRepository;

		Model m_Model;
	};

}
