#include <pch.h>
#include "Board.h"

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

namespace HexChess {

	using ChessEngine::ShaderStage;
	using ChessEngine::ShaderResourceType;
	using ChessEngine::VertexDataType;

	Board::Board(const std::shared_ptr<ChessEngine::Renderer>& renderer)
		: m_Renderer(renderer), m_Generator(m_BoardConfig)
	{
		ChessEngine::PipelineSpecification boardPipelineSpec{};
		boardPipelineSpec.ShaderBinaries[ShaderStage::Vertex] = "Resources/Shaders/BoardShader.vert.spv";
		boardPipelineSpec.ShaderBinaries[ShaderStage::Fragment] = "Resources/Shaders/BoardShader.frag.spv";
		boardPipelineSpec.VertexInput = {
			VertexDataType::Float3,
			VertexDataType::Float3,
			VertexDataType::Float3
		};

		m_Pipeline = m_Renderer->CreatePipeline(boardPipelineSpec);

		m_BoardConfig.IsFlatTopped = true;
		GenerateBoard();

		m_BoardUniforms.ProjectionMatrix = glm::mat4(1.0f);
		m_BoardUniforms.ViewMatrix = glm::mat4(1.0f);
		m_BoardUniforms.ModelMatrix = glm::mat4(1.0f);

		m_UniformBuffer = m_Renderer->CreateUniformBuffer(sizeof(m_BoardUniforms), &m_BoardUniforms);
		m_Pipeline->WriteDescriptor("Uniforms", m_UniformBuffer);
	}

	Board::~Board()
	{
	}

	void Board::OnUpdate(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix)
	{
		if (projectionMatrix != m_CachedProjectionMatrix || viewMatrix != m_CachedViewMatrix)
		{
			m_CachedProjectionMatrix = projectionMatrix;
			m_CachedViewMatrix = viewMatrix;

			m_BoardUniforms.ProjectionMatrix = m_CachedProjectionMatrix;
			m_BoardUniforms.ViewMatrix = m_CachedViewMatrix;
			m_UniformBuffer->SetData(sizeof(m_BoardUniforms), &m_BoardUniforms);
		}

		m_Renderer->BindPipeline(m_Pipeline);
		m_Renderer->BindVertexBuffer(m_VertexBuffer);
		m_Renderer->BindIndexBuffer(m_IndexBuffer);

		m_Renderer->DrawIndexed((uint32_t)m_Generator.GetIndexCount());

		RenderUI();
	}

	void Board::GenerateBoard()
	{
		m_Generator.GenerateBoard();

		if (!m_VertexBuffer)
			m_VertexBuffer = m_Renderer->CreateVertexBuffer(m_Generator.GetVertexCount() * sizeof(BoardVertex), m_Generator.GetVertices().data());
		else
			m_VertexBuffer->SetData(m_Generator.GetVertexCount() * sizeof(BoardVertex), m_Generator.GetVertices().data());

		if (!m_IndexBuffer)
			m_IndexBuffer = m_Renderer->CreateIndexBuffer(m_Generator.GetIndexCount(), m_Generator.GetIndices().data());
		else
			m_IndexBuffer->SetData(m_Generator.GetIndexCount(), m_Generator.GetIndices().data());
	}

	void Board::RenderUI()
	{
		ImGui::Begin("Board configuration");

		bool modified = false;

		modified |= ImGui::DragFloat("Inner size", &m_BoardConfig.InnerSize, 0.05f, 0.001f, m_BoardConfig.OuterSize - glm::epsilon<float>());
		modified |= ImGui::DragFloat("Outer size", &m_BoardConfig.OuterSize, 0.05f, m_BoardConfig.OuterSize - glm::epsilon<float>());
		modified |= ImGui::DragFloat("Height", &m_BoardConfig.Height, 0.05f, glm::epsilon<float>());
		modified |= ImGui::Checkbox("Flat topped", &m_BoardConfig.IsFlatTopped);

		modified |= ImGui::Button("Regenerate board");

		if (modified)
		{
			GenerateBoard();
		}

		ImGui::End();
	}

}
