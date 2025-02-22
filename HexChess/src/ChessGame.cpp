#include <pch.h>
#include "ChessGame.h"

namespace HexChess {

	using ChessEngine::ShaderStage;

	void ChessGame::OnInit()
	{
		m_Renderer = GetRenderer();

		ChessEngine::PipelineSpecification boardPipelineSpec{};
		boardPipelineSpec.ShaderBinaries[ShaderStage::Vertex] = "Resources/Shaders/BoardShader.vert.spv";
		boardPipelineSpec.ShaderBinaries[ShaderStage::Fragment] = "Resources/Shaders/BoardShader.frag.spv";

		m_BoardPipeline = m_Renderer->CreatePipeline(boardPipelineSpec);
	}

	void ChessGame::OnUpdate(float deltaInSeconds)
	{
		m_Renderer->BindPipeline(m_BoardPipeline);
		m_Renderer->Draw(3);
	}

}
