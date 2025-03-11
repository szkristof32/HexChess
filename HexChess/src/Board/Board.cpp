#include <pch.h>
#include "Board.h"

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

namespace HexChess {


	Board::Board(const std::shared_ptr<ChessEngine::Renderer>& renderer)
		: m_Renderer(renderer), m_Generator(m_BoardConfig)
	{
		m_BoardConfig.IsFlatTopped = true;
		GenerateBoard();
	}

	Board::~Board()
	{
	}

	void Board::OnUpdate()
	{
		RenderUI();
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

	void Board::RenderUI()
	{
		ImGui::Begin("Board configuration");

		bool modified = false;

		modified |= ImGui::DragFloat("Inner size", &m_BoardConfig.InnerSize, 0.05f, 0.001f, m_BoardConfig.OuterSize - glm::epsilon<float>());
		modified |= ImGui::DragFloat("Outer size", &m_BoardConfig.OuterSize, 0.05f, m_BoardConfig.OuterSize - glm::epsilon<float>());
		modified |= ImGui::DragFloat("Height", &m_BoardConfig.Height, 0.05f, glm::epsilon<float>());
		//modified |= ImGui::Checkbox("Flat topped", &m_BoardConfig.IsFlatTopped);

		modified |= ImGui::Button("Regenerate board");

		if (modified)
		{
			GenerateBoard();
		}

		ImGui::End();
	}

}
