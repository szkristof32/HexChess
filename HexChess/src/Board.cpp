#include <pch.h>
#include "Board.h"

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>

namespace HexChess {

	using ChessEngine::ShaderStage;
	using ChessEngine::ShaderResourceType;
	using ChessEngine::VertexDataType;

	Board::Board(const std::shared_ptr<ChessEngine::Renderer>& renderer)
		: m_Renderer(renderer)
	{
		ChessEngine::PipelineSpecification boardPipelineSpec{};
		boardPipelineSpec.ShaderBinaries[ShaderStage::Vertex] = "Resources/Shaders/BoardShader.vert.spv";
		boardPipelineSpec.ShaderBinaries[ShaderStage::Fragment] = "Resources/Shaders/BoardShader.frag.spv";
		boardPipelineSpec.VertexInput = {
			VertexDataType::Float3,
			VertexDataType::Float3,
			VertexDataType::Float2
		};

		m_BoardPipeline = m_Renderer->CreatePipeline(boardPipelineSpec);

		const std::vector<BoardVertex> vertices = {
			{ { -0.5f, 0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
			{ { -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },
			{ { 0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
			{ { 0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } }
		};
		m_BoardVertexBuffer = m_Renderer->CreateVertexBuffer(vertices.size() * sizeof(BoardVertex), vertices.data());

		const std::vector<uint32_t> indices = {
			0, 1, 2, 2, 1, 3
		};
		m_BoardIndexBuffer = m_Renderer->CreateIndexBuffer(indices.size(), indices.data());

		m_BoardUniforms.ProjectionMatrix = glm::perspectiveFov(glm::radians(70.0f), 1280.0f, 720.0f, 0.01f, 100.0f);
		m_BoardUniforms.ViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
		m_BoardUniforms.ModelMatrix = glm::mat4(1.0f);

		m_BoardUniformBuffer = m_Renderer->CreateUniformBuffer(sizeof(m_BoardUniforms), &m_BoardUniforms);
		m_BoardPipeline->WriteDescriptor("Uniforms", m_BoardUniformBuffer);

		m_TestImage = m_Renderer->CreateImage("Resources/Images/TestImage.png");
		m_BoardPipeline->WriteDescriptor("TextureSampler", m_TestImage);
	}

	Board::~Board()
	{
	}

	void Board::OnUpdate()
	{
		m_BoardUniforms.ModelMatrix = glm::rotate(m_BoardUniforms.ModelMatrix, glm::radians(1.0f), { 0.0f, 1.0f, 1.0f });
		m_BoardUniformBuffer->SetData(sizeof(m_BoardUniforms), &m_BoardUniforms);

		m_Renderer->BindPipeline(m_BoardPipeline);
		m_Renderer->BindVertexBuffer(m_BoardVertexBuffer);
		m_Renderer->BindIndexBuffer(m_BoardIndexBuffer);

		m_Renderer->DrawIndexed(6);
	}

	void Board::OnResize(uint32_t width, uint32_t height)
	{
		if (width == m_CurrentWidth && height == m_CurrentHeight)
			return;

		m_BoardUniforms.ProjectionMatrix = glm::perspectiveFov(glm::radians(70.0f), (float)width, (float)height, 0.01f, 100.0f);
	}

}
