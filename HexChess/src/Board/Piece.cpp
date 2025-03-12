#include <pch.h>
#include "Piece.h"

namespace HexChess {

	namespace PieceUtils {

		static std::filesystem::path PieceTypeModelPath(PieceType type)
		{
			if (type & PieceType::Pawn)		return "Resources/Models/Pawn.glb";
			if (type & PieceType::Knight)	return "Resources/Models/Knight.glb";
			if (type & PieceType::Bishop)	return "Resources/Models/Bishop.glb";
			if (type & PieceType::Rook)		return "Resources/Models/Rook.glb";
			if (type & PieceType::Queen)	return "Resources/Models/Queen.glb";
			if (type & PieceType::King)		return "Resources/Models/King.glb";

			return "";
		}

	}

	Piece::Piece(PieceType type, ModelRepository& modelRepo)
		: m_Type(type)
	{
		m_Model = modelRepo.GetModel(PieceUtils::PieceTypeModelPath(m_Type));
	}

	Piece::~Piece()
	{
	}

}
