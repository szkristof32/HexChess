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

		const std::vector<Move>& GenerateMoves(const Piece& piece);
	private:
		void GeneratePawnMoves(const Piece& piece);
		void GenerateKnightMoves(const Piece& piece);
		void GenerateBishopMoves(const Piece& piece);
		void GenerateRookMoves(const Piece& piece);
		void GenerateQueenMoves(const Piece& piece);
		void GenerateKingMoves(const Piece& piece);

		PieceType PieceAt(uint32_t file, uint32_t rank) const;
		bool IsPieceAtAny(uint32_t file, uint32_t rank, const std::vector<glm::vec2>& positions) const;
		uint32_t GetRankCentre(uint32_t rank) const { return rank <= 5 ? 5 : 10 - rank; }
		uint32_t GetRankLength(uint32_t rank) const { return rank <= 5 ? 11 : 21 - (2 * rank); }
		void AddMove(const Piece& piece, uint32_t fileOffset, uint32_t rankOffset, bool relativeToCentre = true);
	private:
		const PieceCollection& m_Pieces;

		std::vector<Move> m_Moves;
	};

}
