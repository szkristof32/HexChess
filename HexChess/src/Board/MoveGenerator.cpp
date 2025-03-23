#include <pch.h>
#include "MoveGenerator.h"

namespace HexChess {

	MoveGenerator::MoveGenerator(const PieceCollection& pieces)
		: m_Pieces(pieces)
	{
	}

	MoveGenerator::~MoveGenerator()
	{
	}

	std::vector<Move> MoveGenerator::GenerateMoves(const Piece& piece)
	{
		PieceType type = piece.GetPieceType() & ~(PieceType::White | PieceType::Black);

		switch (type)
		{
			case PieceType::Pawn:	return GeneratePawnMoves(piece);
			case PieceType::Knight:	return GenerateKnightMoves(piece);
			case PieceType::Bishop:	return GenerateBishopMoves(piece);
			case PieceType::Rook:	return GenerateRookMoves(piece);
			case PieceType::Queen:	return GenerateQueenMoves(piece);
			case PieceType::King:	return GenerateKingMoves(piece);
		}

		return {};
	}

	std::vector<Move> MoveGenerator::GeneratePawnMoves(const Piece& piece)
	{
		PieceType type = piece.GetPieceType() & ~(PieceType::White | PieceType::Black);
		PieceType colour = piece.GetPieceType() & (PieceType::White | PieceType::Black);

		uint32_t file = piece.GetFile();
		uint32_t rank = piece.GetRank();

		std::vector<Move> moves;

		if (colour & PieceType::White)
		{
			if (rank < 5)
			{
				// Basic forward movement
				if (PieceAt(file, rank + 1) == PieceType::None)
					moves.emplace_back(glm::vec2(file, rank), glm::vec2(file, rank + 1));

				// First square double movement
				if (IsPieceAtAny(file, rank, PieceContstants::PawnStartLocationsWhite) && rank < 4
					&& PieceAt(file, rank + 2) == PieceType::None)
					moves.emplace_back(glm::vec2(file, rank), glm::vec2(file, rank + 2));

				if (IsPieceAtAny(file, rank, PieceContstants::PawnStartLocationsWhite) && rank == 4 
					&& PieceAt(file - 1, rank + 2) == PieceType::None)
					moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 1, rank + 2));

				// Capturing
				if (file < 5)
				{
					if (PieceAt(file - 1, rank) & PieceType::Black)
						moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 1, rank));

					if (PieceAt(file + 1, rank + 1) & PieceType::Black)
						moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 1, rank + 1));
				}
				else if (file == 5)
				{
					if (PieceAt(file - 1, rank) & PieceType::Black)
						moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 1, rank));

					if (PieceAt(file + 1, rank) & PieceType::Black)
						moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 1, rank));
				}
				else
				{
					if (PieceAt(file - 1, rank + 1) & PieceType::Black)
						moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 1, rank + 1));

					if (PieceAt(file + 1, rank) & PieceType::Black)
						moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 1, rank));
				}
			}
			else
			{
				// Basic forward movement
				if (PieceAt(file - 1, rank + 1) == PieceType::None)
					moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 1, rank + 1));

				uint32_t rankCentre = 10 - rank;

				// Capturing
				if (file < rankCentre)
				{
					if (PieceAt(file - 1, rank) & PieceType::Black)
						moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 1, rank));

					if (PieceAt(file, rank + 1) & PieceType::Black)
						moves.emplace_back(glm::vec2(file, rank), glm::vec2(file, rank + 1));
				}
				else if (file == rankCentre)
				{
					if (PieceAt(file - 1, rank) & PieceType::Black)
						moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 1, rank));

					if (PieceAt(file + 1, rank) & PieceType::Black)
						moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 1, rank));
				}
				else
				{
					if (PieceAt(file - 2, rank + 1) & PieceType::Black)
						moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 2, rank + 1));

					if (PieceAt(file + 1, rank) & PieceType::Black)
						moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 1, rank));
				}
			}
		}
		else
		{
			if (rank <= 5)
			{
				// Basic forward movement
				if (PieceAt(file, rank - 1) == PieceType::None)
					moves.emplace_back(glm::vec2(file, rank), glm::vec2(file, rank - 1));

				// Capturing
				if (file < 5)
				{
					if (PieceAt(file - 1, rank - 1) & PieceType::White)
						moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 1, rank - 1));

					if (PieceAt(file + 1, rank) & PieceType::White)
						moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 1, rank));
				}
				else if (file == 5)
				{
					if (PieceAt(file - 1, rank - 1) & PieceType::White)
						moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 1, rank - 1));

					if (PieceAt(file + 1, rank - 1) & PieceType::White)
						moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 1, rank - 1));
				}
				else
				{
					if (PieceAt(file - 1, rank) & PieceType::White)
						moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 1, rank));

					if (PieceAt(file + 1, rank - 1) & PieceType::White)
						moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 1, rank - 1));
				}
			}
			else
			{
				// Basic forward movement
				if (PieceAt(file + 1, rank - 1) == PieceType::None)
					moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 1, rank - 1));

				// First square double movement
				if (IsPieceAtAny(file, rank, PieceContstants::PawnStartLocationsBlack)
					&& PieceAt(file + 1, rank - 2) == PieceType::None)
					moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 1, rank - 2));

				uint32_t rankCentre = 10 - rank;

				// Capturing
				if (file < rankCentre)
				{
					if (PieceAt(file, rank - 1) & PieceType::Black)
						moves.emplace_back(glm::vec2(file, rank), glm::vec2(file, rank - 1));

					if (PieceAt(file + 1, rank) & PieceType::Black)
						moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 1, rank));
				}
				else if (file == rankCentre)
				{
					if (PieceAt(file, rank - 1) & PieceType::Black)
						moves.emplace_back(glm::vec2(file, rank), glm::vec2(file, rank - 1));

					if (PieceAt(file + 2, rank - 1) & PieceType::Black)
						moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 2, rank - 1));
				}
				else
				{
					if (PieceAt(file - 1, rank) & PieceType::Black)
						moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 1, rank));

					if (PieceAt(file + 2, rank - 1) & PieceType::Black)
						moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 2, rank - 1));
				}
			}
		}

		return moves;
	}

	std::vector<Move> MoveGenerator::GenerateKnightMoves(const Piece& piece)
	{
		return {};
	}

	std::vector<Move> MoveGenerator::GenerateBishopMoves(const Piece& piece)
	{
		return {};
	}

	std::vector<Move> MoveGenerator::GenerateRookMoves(const Piece& piece)
	{
		return {};
	}

	std::vector<Move> MoveGenerator::GenerateQueenMoves(const Piece& piece)
	{
		return {};
	}

	std::vector<Move> MoveGenerator::GenerateKingMoves(const Piece& piece)
	{
		return {};
	}

	PieceType MoveGenerator::PieceAt(uint32_t file, uint32_t rank) const
	{
		if (m_Pieces.contains(glm::vec2(file, rank)))
			return m_Pieces.at(glm::vec2(file, rank)).GetPieceType();

		return PieceType::None;
	}

	bool MoveGenerator::IsPieceAtAny(uint32_t file, uint32_t rank, const std::vector<glm::vec2>& positions) const
	{
		return std::find(positions.begin(), positions.end(), glm::vec2(file, rank)) != positions.end();
	}

}
