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

		m_BoardUniforms.ProjectionMatrix = glm::perspectiveFov(glm::radians(70.0f), 1280.0f, 720.0f, 0.01f, 100.0f);
		m_BoardUniforms.ViewMatrix = glm::lookAt(glm::vec3(0.0f, 5.0f, 5.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		m_BoardUniforms.ModelMatrix = glm::mat4(1.0f);

		m_UniformBuffer = m_Renderer->CreateUniformBuffer(sizeof(m_BoardUniforms), &m_BoardUniforms);
		m_Pipeline->WriteDescriptor("Uniforms", m_UniformBuffer);
	}

	Board::~Board()
	{
	}

	void Board::OnUpdate()
	{
		m_Renderer->BindPipeline(m_Pipeline);
		m_Renderer->BindVertexBuffer(m_VertexBuffer);
		m_Renderer->BindIndexBuffer(m_IndexBuffer);

		m_Renderer->DrawIndexed((uint32_t)m_Generator.GetIndexCount());

		RenderUI();
	}

	void Board::OnResize(uint32_t width, uint32_t height)
	{
		if (width == m_CurrentWidth && height == m_CurrentHeight)
			return;

		m_BoardUniforms.ProjectionMatrix = glm::perspectiveFov(glm::radians(70.0f), (float)width, (float)height, 0.01f, 100.0f);
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
