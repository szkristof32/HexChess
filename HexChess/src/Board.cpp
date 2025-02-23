#include <pch.h>
#include "Board.h"

namespace HexChess {

	using ChessEngine::ShaderStage;
	using ChessEngine::VertexDataType;

	Board::Board(const std::shared_ptr<ChessEngine::Renderer>& renderer)
		: m_Renderer(renderer)
	{
		ChessEngine::PipelineSpecification boardPipelineSpec{};
		boardPipelineSpec.ShaderBinaries[ShaderStage::Vertex] = "Resources/Shaders/BoardShader.vert.spv";
		boardPipelineSpec.ShaderBinaries[ShaderStage::Fragment] = "Resources/Shaders/BoardShader.frag.spv";
		boardPipelineSpec.VertexInput = {
			VertexDataType::Float3,
			VertexDataType::Float3
		};

		m_BoardPipeline = m_Renderer->CreatePipeline(boardPipelineSpec);

		const std::vector<BoardVertex> vertices = {
			{ { -0.5f, 0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
			{ { 0.5f, 0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
			{ { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
			{ { 0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f } }
		};
		m_BoardVertexBuffer = m_Renderer->CreateVertexBuffer(vertices.size() * sizeof(BoardVertex), vertices.data());

		const std::vector<uint32_t> indices = {
			0, 1, 2, 2, 1, 3
		};
		m_BoardIndexBuffer = m_Renderer->CreateIndexBuffer(indices.size(), indices.data());
	}

	Board::~Board()
	{
	}

	void Board::OnUpdate()
	{
		m_Renderer->BindPipeline(m_BoardPipeline);
		m_Renderer->BindVertexBuffer(m_BoardVertexBuffer);
		m_Renderer->BindIndexBuffer(m_BoardIndexBuffer);

		m_Renderer->DrawIndexed(6);
	}

}
