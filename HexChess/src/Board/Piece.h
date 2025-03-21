#pragma once

#include "Model.h"
#include "Loading/ModelRepository.h"

#include <ChessEngine/Utils/EnumUtils.h>

namespace HexChess {

	enum class PieceType
	{
		None = 0,

		Pawn = 0x1,
		Knight = 0x2,
		Bishop = 0x4,
		Rook = 0x8,
		Queen = 0x10,
		King = 0x20,

		White = 0x100,
		Black = 0x200
	};

	class Piece
	{
	public:
		Piece() = default;
		Piece(PieceType type, ModelRepository& modelRepo);
		~Piece();

		const Model& GetModel() const { return m_Model; }
		PieceType GetPieceType() const { return m_Type; }

		uint32_t GetFile() const { return m_File; }
		void SetFile(uint32_t file) { m_File = file; }
		uint32_t GetRank() const { return m_Rank; }
		void SetRank(uint32_t rank) { m_Rank = rank; }
	private:
		PieceType m_Type;
		Model m_Model;

		uint32_t m_File = 0;
		uint32_t m_Rank = 0;
	};

}
