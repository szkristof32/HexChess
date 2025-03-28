#include <pch.h>
#include "BoardRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace HexChess {

	using ChessEngine::ShaderStage;
	using ChessEngine::VertexDataType;

	BoardRenderer::BoardRenderer(const std::shared_ptr<ChessEngine::Renderer>& renderer)
		: m_Renderer(renderer)
	{
		ChessEngine::PipelineSpecification pipelineSpec{};
		pipelineSpec.ShaderBinaries[ShaderStage::Vertex] = "Resources/Shaders/BoardShader.vert.spv";
		pipelineSpec.ShaderBinaries[ShaderStage::Fragment] = "Resources/Shaders/BoardShader.frag.spv";
		pipelineSpec.VertexInput = {
			VertexDataType::Float3,
			VertexDataType::Float3,
			VertexDataType::Float3,
			VertexDataType::Float2
		};

		m_Pipeline = m_Renderer->CreatePipeline(pipelineSpec);

		m_BoardUniforms.ProjectionMatrix = glm::mat4(1.0f);
		m_BoardUniforms.ViewMatrix = glm::mat4(1.0f);
		m_BoardUniforms.ModelMatrix = glm::mat4(1.0f);

		m_UniformBuffer = m_Renderer->CreateUniformBuffer(sizeof(m_BoardUniforms), &m_BoardUniforms);
		m_Pipeline->WriteDescriptor("Uniforms", m_UniformBuffer);

		m_StorageBufferMarking = m_Renderer->CreateStorageBuffer(sizeof(m_BoardMarking), &m_BoardMarking);
		m_Pipeline->WriteDescriptor("Marking", m_StorageBufferMarking);
	}

	BoardRenderer::~BoardRenderer()
	{
	}

	void BoardRenderer::BeginFrame(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix)
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
	}

	void BoardRenderer::EndFrame()
	{
	}

	void BoardRenderer::RenderBoard(const Board& board)
	{
		const auto& model = board.GetModel();

		const auto& moves = board.GetCurrentMoves();
		for (uint32_t i = 0;i < 42;i++)
		{
			glm::vec2 cell = i < moves.size() ? moves[i].Destination : glm::vec2(1000.0f);

			if (i % 2 == 0)
			{
				m_BoardMarking.MoveCells[i / 2].x = cell.x;
				m_BoardMarking.MoveCells[i / 2].y = cell.y;
			}
			else
			{
				m_BoardMarking.MoveCells[i / 2].z = cell.x;
				m_BoardMarking.MoveCells[i / 2].w = cell.y;
			}
		}
		m_StorageBufferMarking->SetData(sizeof(m_BoardMarking), &m_BoardMarking);

		m_Renderer->BindVertexBuffer(model.VertexBuffer);
		m_Renderer->BindIndexBuffer(model.IndexBuffer);

		m_Renderer->DrawIndexed(model.IndexCount);
	}

}
