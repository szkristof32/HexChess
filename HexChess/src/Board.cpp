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
			{ { 0.0f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
			{ { 0.5f, 0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
			{ { -0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f } }
		};
	}

	Board::~Board()
	{
	}

	void Board::OnUpdate()
	{
		m_Renderer->BindPipeline(m_BoardPipeline);
		m_Renderer->Draw(3);
	}

}
