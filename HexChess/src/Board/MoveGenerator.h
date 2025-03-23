#pragma once

#include "Board/Piece.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

namespace HexChess {

	class MoveGenerator
	{
	public:
		using PieceCollection = std::unordered_map<glm::vec2, Piece>;
	public:
		MoveGenerator(const PieceCollection& pieces);
		~MoveGenerator();

		std::vector<Move> GenerateMoves(const Piece& piece);
	private:
		std::vector<Move> GeneratePawnMoves(const Piece& piece);
		std::vector<Move> GenerateKnightMoves(const Piece& piece);
		std::vector<Move> GenerateBishopMoves(const Piece& piece);
		std::vector<Move> GenerateRookMoves(const Piece& piece);
		std::vector<Move> GenerateQueenMoves(const Piece& piece);
		std::vector<Move> GenerateKingMoves(const Piece& piece);

		PieceType PieceAt(uint32_t file, uint32_t rank) const;
		bool IsPieceAtAny(uint32_t file, uint32_t rank, const std::vector<glm::vec2>& positions) const;
	private:
		const PieceCollection& m_Pieces;
	};

}
