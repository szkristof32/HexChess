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

	const std::vector<Move>& MoveGenerator::GenerateMoves(const Piece& piece)
	{
		PieceType type = piece.GetPieceType() & ~(PieceType::White | PieceType::Black);
		m_Moves.clear();

		switch (type)
		{
			case PieceType::Pawn:	GeneratePawnMoves(piece); break;
			case PieceType::Knight:	GenerateKnightMoves(piece); break;
			case PieceType::Bishop:	GenerateBishopMoves(piece); break;
			case PieceType::Rook:	GenerateRookMoves(piece); break;
			case PieceType::Queen:	GenerateQueenMoves(piece); break;
			case PieceType::King:	GenerateKingMoves(piece); break;
		}

		return m_Moves;
	}

	void MoveGenerator::GeneratePawnMoves(const Piece& piece)
	{
		PieceType type = piece.GetPieceType() & ~(PieceType::White | PieceType::Black);
		PieceType colour = piece.GetPieceType() & (PieceType::White | PieceType::Black);

		uint32_t file = piece.GetFile();
		uint32_t rank = piece.GetRank();

		if (colour & PieceType::White)
		{
			if (rank < 5)
			{
				// Basic forward movement
				if (PieceAt(file, rank + 1) == PieceType::None)
					m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file, rank + 1));

				// First square double movement
				if (IsPieceAtAny(file, rank, PieceContstants::PawnStartLocationsWhite) && rank < 4
					&& PieceAt(file, rank + 2) == PieceType::None)
					m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file, rank + 2));

				if (IsPieceAtAny(file, rank, PieceContstants::PawnStartLocationsWhite) && rank == 4
					&& PieceAt(file - 1, rank + 2) == PieceType::None)
					m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 1, rank + 2));

				// Capturing
				if (file < 5)
				{
					if (PieceAt(file - 1, rank) & PieceType::Black)
						m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 1, rank));

					if (PieceAt(file + 1, rank + 1) & PieceType::Black)
						m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 1, rank + 1));
				}
				else if (file == 5)
				{
					if (PieceAt(file - 1, rank) & PieceType::Black)
						m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 1, rank));

					if (PieceAt(file + 1, rank) & PieceType::Black)
						m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 1, rank));
				}
				else
				{
					if (PieceAt(file - 1, rank + 1) & PieceType::Black)
						m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 1, rank + 1));

					if (PieceAt(file + 1, rank) & PieceType::Black)
						m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 1, rank));
				}
			}
			else
			{
				// Basic forward movement
				if (PieceAt(file - 1, rank + 1) == PieceType::None)
					m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 1, rank + 1));

				uint32_t rankCentre = 10 - rank;

				// Capturing
				if (file < rankCentre)
				{
					if (PieceAt(file - 1, rank) & PieceType::Black)
						m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 1, rank));

					if (PieceAt(file, rank + 1) & PieceType::Black)
						m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file, rank + 1));
				}
				else if (file == rankCentre)
				{
					if (PieceAt(file - 1, rank) & PieceType::Black)
						m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 1, rank));

					if (PieceAt(file + 1, rank) & PieceType::Black)
						m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 1, rank));
				}
				else
				{
					if (PieceAt(file - 2, rank + 1) & PieceType::Black)
						m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 2, rank + 1));

					if (PieceAt(file + 1, rank) & PieceType::Black)
						m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 1, rank));
				}
			}
		}
		else
		{
			if (rank <= 5)
			{
				// Basic forward movement
				if (PieceAt(file, rank - 1) == PieceType::None)
					m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file, rank - 1));

				// Capturing
				if (file < 5)
				{
					if (PieceAt(file - 1, rank - 1) & PieceType::White)
						m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 1, rank - 1));

					if (PieceAt(file + 1, rank) & PieceType::White)
						m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 1, rank));
				}
				else if (file == 5)
				{
					if (PieceAt(file - 1, rank - 1) & PieceType::White)
						m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 1, rank - 1));

					if (PieceAt(file + 1, rank - 1) & PieceType::White)
						m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 1, rank - 1));
				}
				else
				{
					if (PieceAt(file - 1, rank) & PieceType::White)
						m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 1, rank));

					if (PieceAt(file + 1, rank - 1) & PieceType::White)
						m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 1, rank - 1));
				}
			}
			else
			{
				// Basic forward movement
				if (PieceAt(file + 1, rank - 1) == PieceType::None)
					m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 1, rank - 1));

				// First square double movement
				if (IsPieceAtAny(file, rank, PieceContstants::PawnStartLocationsBlack)
					&& PieceAt(file + 1, rank - 2) == PieceType::None)
					m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 1, rank - 2));

				uint32_t rankCentre = GetRankCentre(rank);

				// Capturing
				if (file < rankCentre)
				{
					if (PieceAt(file, rank - 1) & PieceType::Black)
						m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file, rank - 1));

					if (PieceAt(file + 1, rank) & PieceType::Black)
						m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 1, rank));
				}
				else if (file == rankCentre)
				{
					if (PieceAt(file, rank - 1) & PieceType::Black)
						m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file, rank - 1));

					if (PieceAt(file + 2, rank - 1) & PieceType::Black)
						m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 2, rank - 1));
				}
				else
				{
					if (PieceAt(file - 1, rank) & PieceType::Black)
						m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 1, rank));

					if (PieceAt(file + 2, rank - 1) & PieceType::Black)
						m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 2, rank - 1));
				}
			}
		}
	}

	void MoveGenerator::GenerateKnightMoves(const Piece& piece)
	{
		PieceType type = piece.GetPieceType() & ~(PieceType::White | PieceType::Black);
		PieceType colour = piece.GetPieceType() & (PieceType::White | PieceType::Black);
		PieceType oppositionColour = colour & PieceType::White ? PieceType::Black : PieceType::White;

		uint32_t file = piece.GetFile();
		uint32_t rank = piece.GetRank();

		uint32_t rankCentre = GetRankCentre(rank);

		if (file < rankCentre)
		{
			if ((PieceAt(file - 1, rank + 2) & colour) != colour)
				m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 1, rank + 2));
			if ((PieceAt(file + 1, rank + 3) & colour) != colour)
				m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 1, rank + 3));

			if ((PieceAt(file - 2, rank + 1) & colour) != colour)
				m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 2, rank + 1));
			if ((PieceAt(file + 2, rank + 3) & colour) != colour)
				m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 2, rank + 3));

			if ((PieceAt(file - 3, rank - 1) & colour) != colour)
				m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 3, rank - 1));
			if ((PieceAt(file + 3, rank + 1) & colour) != colour)
				m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 3, rank + 1));

			if ((PieceAt(file - 3, rank - 2) & colour) != colour)
				m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 3, rank - 2));
			if ((PieceAt(file + 3, rank) & colour) != colour)
				m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 3, rank));

			if ((PieceAt(file - 2, rank - 3) & colour) != colour)
				m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 2, rank - 3));
			if ((PieceAt(file + 2, rank - 1) & colour) != colour)
				m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 2, rank - 1));

			if ((PieceAt(file - 1, rank - 3) & colour) != colour)
				m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file - 1, rank - 3));
			if ((PieceAt(file + 1, rank - 2) & colour) != colour)
				m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + 1, rank - 2));
		}
		else if (file == rankCentre)
		{
			if ((PieceAt(GetRankCentre(rank + 2) - 1, rank + 2) & colour) != colour)
				m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(GetRankCentre(rank + 2) - 1, rank + 2));
			if ((PieceAt(GetRankCentre(rank + 2) + 1, rank + 2) & colour) != colour)
				m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(GetRankCentre(rank + 2) + 1, rank + 2));

			if ((PieceAt(GetRankCentre(rank + 1) - 2, rank + 1) & colour) != colour)
				m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(GetRankCentre(rank + 1) - 2, rank + 1));
			if ((PieceAt(GetRankCentre(rank + 1) + 2, rank + 1) & colour) != colour)
				m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(GetRankCentre(rank + 1) + 2, rank + 1));

			if ((PieceAt(GetRankCentre(rank - 1) - 3, rank - 1) & colour) != colour)
				m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(GetRankCentre(rank - 1) - 3, rank - 1));
			if ((PieceAt(GetRankCentre(rank - 1) + 3, rank - 1) & colour) != colour)
				m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(GetRankCentre(rank - 1) + 3, rank - 1));

			if ((PieceAt(GetRankCentre(rank - 2) - 3, rank - 2) & colour) != colour)
				m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(GetRankCentre(rank - 2) - 3, rank - 2));
			if ((PieceAt(GetRankCentre(rank - 2) + 3, rank - 2) & colour) != colour)
				m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(GetRankCentre(rank - 2) + 3, rank - 2));

			if ((PieceAt(GetRankCentre(rank - 3) - 2, rank - 3) & colour) != colour)
				m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(GetRankCentre(rank - 3) - 2, rank - 3));
			if ((PieceAt(GetRankCentre(rank - 3) + 2, rank - 3) & colour) != colour)
				m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(GetRankCentre(rank - 3) + 2, rank - 3));

			if ((PieceAt(GetRankCentre(rank - 3) - 1, rank - 3) & colour) != colour)
				m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(GetRankCentre(rank - 3) - 1, rank - 3));
			if ((PieceAt(GetRankCentre(rank - 3) + 1, rank - 3) & colour) != colour)
				m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(GetRankCentre(rank - 3) + 1, rank - 3));
		}
		else
		{
			if (rank < 6 || true)
			{
				AddMove(piece, -1, 2);
				AddMove(piece, 1, 2);

				AddMove(piece, -2, 3);
				AddMove(piece, 2, 1);

				AddMove(piece, -3, 1);
				AddMove(piece, 3, -1);

				AddMove(piece, -3, 0);
				AddMove(piece, 3, -2);

				AddMove(piece, -2, -1);
				AddMove(piece, 2, -3);

				AddMove(piece, -1, -2);
				AddMove(piece, 1, -3);
			}
		}
	}

	void MoveGenerator::GenerateBishopMoves(const Piece& piece)
	{
	}

	void MoveGenerator::GenerateRookMoves(const Piece& piece)
	{
	}

	void MoveGenerator::GenerateQueenMoves(const Piece& piece)
	{
	}

	void MoveGenerator::GenerateKingMoves(const Piece& piece)
	{
	}

	PieceType MoveGenerator::PieceAt(uint32_t file, uint32_t rank) const
	{
		if (m_Pieces.contains(glm::vec2(file, rank)))
			return m_Pieces.at(glm::vec2(file, rank)).GetPieceType();

		return PieceType::None;
	}

	bool MoveGenerator::IsPieceAtAny(uint32_t file, uint32_t rank, const std::vector<glm::vec2>& positions) const
	{
		if (file < 0 || file >= GetRankLength(rank) || rank < 0 || rank >= 11)
			return false;

		return std::find(positions.begin(), positions.end(), glm::vec2(file, rank)) != positions.end();
	}

	void MoveGenerator::AddMove(const Piece& piece, uint32_t fileOffset, uint32_t rankOffset, bool relativeToCentre)
	{
		uint32_t file = piece.GetFile();
		uint32_t rank = piece.GetRank();
		PieceType colour = piece.GetPieceType() & (PieceType::White | PieceType::Black);

		if (relativeToCentre)
		{
			uint32_t rankCentreCurrent = GetRankCentre(rank);
			int32_t offsetCurrent = file - rankCentreCurrent;
			uint32_t rankCentreNew = GetRankCentre(rank + rankOffset);
			int32_t offsetNew = offsetCurrent + fileOffset;

			if (glm::sign(offsetCurrent) == glm::sign(offsetNew))
				rankOffset++;

			if ((PieceAt(rankCentreNew + offsetNew, rank + rankOffset) & colour) != colour)
				m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(rankCentreNew + offsetNew, rank + rankOffset));
		}
		else
		{
			if ((PieceAt(file + fileOffset, rank + rankOffset) & colour) != colour)
				m_Moves.emplace_back(glm::vec2(file, rank), glm::vec2(file + fileOffset, rank + rankOffset));
		}
	}

}
