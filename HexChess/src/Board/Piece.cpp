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

		static glm::vec3 PieceTypeColour(PieceType type)
		{
			if (type & PieceType::White)	return glm::vec3(0.95f);
			if (type & PieceType::Black)	return glm::vec3(0.15f);

			return { 1.0f, 0.0f, 1.0f };
		}

	}

	Piece::Piece(PieceType type, ModelRepository& modelRepo)
		: m_Type(type)
	{
		m_Model = modelRepo.GetModel(PieceUtils::PieceTypeModelPath(m_Type), PieceUtils::PieceTypeColour(m_Type));
	}

	Piece::~Piece()
	{
	}

}
