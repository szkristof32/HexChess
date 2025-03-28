#include <pch.h>
#include "Board.h"

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

namespace HexChess {

	namespace PieceUtils {

		static PieceType GetPieceColour(char piece)
		{
			return piece >= 'a' && piece <= 'z' ? PieceType::Black : PieceType::White;
		}

		static PieceType GetPieceType(char piece)
		{
			piece = piece >= 'a' && piece <= 'z' ? piece - 'a' + 'A' : piece;

			switch (piece)
			{
				case 'P':	return PieceType::Pawn;
				case 'B':	return PieceType::Bishop;
				case 'N':	return PieceType::Knight;
				case 'R':	return PieceType::Rook;
				case 'Q':	return PieceType::Queen;
				case 'K':	return PieceType::King;
			}

			return PieceType::None;
		}

	}

	Board::Board(const std::shared_ptr<ChessEngine::Renderer>& renderer, const std::shared_ptr<ModelRepository>& modelRepository)
		: m_Renderer(renderer), m_Generator(m_BoardConfig), m_ModelRepository(modelRepository), m_MoveGenerator(m_Pieces)
	{
		m_BoardConfig.IsFlatTopped = true;
		GenerateBoard();
		GeneratePieces();
	}

	Board::~Board()
	{
	}

	void Board::OnUpdate()
	{
		RenderUI();
	}

	Piece& Board::GetPieceAt(uint32_t file, uint32_t rank)
	{
		if (m_Pieces.contains({ file, rank }))
			return m_Pieces.at({ file, rank });

		return m_NullPiece;
	}

	bool Board::TryMakeMove(const Move& move)
	{
		if (!GetPieceAt((uint32_t)move.Start.x, (uint32_t)move.Start.y).IsValid())
			return false;

		m_CurrentMoves = m_MoveGenerator.GenerateMoves(m_Pieces.at(move.Start));
		if (std::find(m_CurrentMoves.begin(), m_CurrentMoves.end(), move) == m_CurrentMoves.end())
		{
			m_CurrentMoves.clear();
			return false;
		}

		Piece& piece = m_Pieces[move.Destination] = m_Pieces.at(move.Start);
		piece.SetFile((uint32_t)move.Destination.x);
		piece.SetRank((uint32_t)move.Destination.y);
		m_Pieces.erase(move.Start);

		m_CurrentMoves.clear();

		return true;
	}

	void Board::GenerateBoard()
	{
		m_Generator.GenerateBoard();

		if (!m_Model.VertexBuffer)
			m_Model.VertexBuffer = m_Renderer->CreateVertexBuffer(m_Generator.GetVertexCount() * sizeof(BoardVertex), m_Generator.GetVertices().data());
		else
			m_Model.VertexBuffer->SetData(m_Generator.GetVertexCount() * sizeof(BoardVertex), m_Generator.GetVertices().data());

		if (!m_Model.IndexBuffer)
			m_Model.IndexBuffer = m_Renderer->CreateIndexBuffer(m_Generator.GetIndexCount(), m_Generator.GetIndices().data());
		else
			m_Model.IndexBuffer->SetData(m_Generator.GetIndexCount(), m_Generator.GetIndices().data());

		m_Model.IndexCount = (uint32_t)m_Generator.GetIndexCount();
	}

	void Board::GeneratePieces(std::string_view fen)
	{
		const char* fenData = fen.data();

		uint32_t file = 0;
		uint32_t rank = 10;

		for (size_t i = 0;i < fen.size();i++)
		{
			char position = fenData[i];

			if (position == '/')
			{
				file = 0;
				rank--;
				continue;
			}
			if (position >= '0' && position <= '9')
			{
				uint32_t skip = (uint32_t)(position - '0');
				file += skip;
				continue;
			}
			if (position < 'A' || (position > 'Z' && position < 'a') || position > 'z')
			{
				continue;
			}

			PieceType pieceType = PieceUtils::GetPieceColour(position) | PieceUtils::GetPieceType(position);

			Piece& piece = m_Pieces[{file, rank}] = Piece(pieceType, *m_ModelRepository);
			piece.SetFile(file);
			piece.SetRank(rank);

			file++;
		}
	}

	void Board::RenderUI()
	{
		ImGui::Begin("Board configuration");

		bool modified = false;

		modified |= ImGui::DragFloat("Inner size", &m_BoardConfig.InnerSize, 0.05f, 0.001f, m_BoardConfig.OuterSize - glm::epsilon<float>());
		modified |= ImGui::DragFloat("Outer size", &m_BoardConfig.OuterSize, 0.05f, m_BoardConfig.OuterSize - glm::epsilon<float>());
		modified |= ImGui::DragFloat("Height", &m_BoardConfig.Height, 0.05f, glm::epsilon<float>());

		ImGui::Separator();

		modified |= ImGui::ColorEdit3("Light colour", &m_BoardConfig.Colours[0].r);
		modified |= ImGui::ColorEdit3("Medium colour", &m_BoardConfig.Colours[1].r);
		modified |= ImGui::ColorEdit3("Dark colour", &m_BoardConfig.Colours[2].r);

		if (modified)
		{
			GenerateBoard();
		}

		ImGui::End();
	}

}
