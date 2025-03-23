#pragma once

#include "Model.h"
#include "Loading/ModelRepository.h"

#include <ChessEngine/Utils/EnumUtils.h>

namespace HexChess {

	struct Move
	{
		glm::vec2 Start;
		glm::vec2 Destination;

		inline bool operator==(const Move& other) const
		{
			return Start == other.Start && Destination == other.Destination;
		}
		inline bool operator!=(const Move& other) const
		{
			return !(*this == other);
		}
	};

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

		bool IsValid() const { return m_Type != PieceType::None; }
		void Select(bool select = true) { m_Selected = select; }
		bool IsSelected() const { return m_Selected; }
	private:
		PieceType m_Type = PieceType::None;
		Model m_Model;

		uint32_t m_File = 0;
		uint32_t m_Rank = 0;

		bool m_Selected = false;
	};

	namespace PieceContstants {

		const inline std::vector<glm::vec2> PawnStartLocationsWhite = {
			{ 1, 0 }, { 2, 1 }, { 3, 2 }, { 4, 3 }, { 5, 4 },
			{ 9, 0 }, { 8, 1 }, { 7, 2 }, { 6, 3 }
		};
		const inline std::vector<glm::vec2> PawnStartLocationsBlack = {
			{ 0, 6 }, { 1, 6 }, { 2, 6 }, { 3, 6 }, { 4, 6 },
			{ 8, 6 }, { 7, 6 }, { 6, 6 }, { 5, 6 }
		};

	}

}
