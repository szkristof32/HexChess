#pragma once

#include "Model.h"

#include "Board/Piece.h"
#include "Board/BoardGenerator.h"
#include "Board/MoveGenerator.h"

#include <ChessEngine/Core/Window.h>
#include <ChessEngine/Core/Input.h>
#include <ChessEngine/Rendering/Renderer.h>
#include <ChessEngine/Rendering/Buffers.h>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

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
		BoardGenerator& GetGenerator() { return m_Generator; }

		const std::unordered_map<glm::vec2, Piece>& GetPieces() const { return m_Pieces; }
		Piece& GetPieceAt(uint32_t file, uint32_t rank);
		bool TryMakeMove(const Move& move);
	private:
		void GenerateBoard();
		void GeneratePieces(std::string_view fen = DefaultFEN);

		void RenderUI();
	private:
		std::shared_ptr<ChessEngine::Renderer> m_Renderer;

		BoardConfig m_BoardConfig;
		BoardGenerator m_Generator;

		Piece m_NullPiece;
		std::unordered_map<glm::vec2, Piece> m_Pieces;
		MoveGenerator m_MoveGenerator;
		std::shared_ptr<ModelRepository> m_ModelRepository;

		Model m_Model;
	};

}
