#include <pch.h>
#include "PieceRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace HexChess {

	using ChessEngine::ShaderStage;
	using ChessEngine::VertexDataType;

	PieceRenderer::PieceRenderer(const std::shared_ptr<ChessEngine::Renderer>& renderer)
		: m_Renderer(renderer)
	{
		ChessEngine::PipelineSpecification pipelineSpec{};
		pipelineSpec.ShaderBinaries[ShaderStage::Vertex] = "Resources/Shaders/PieceShader.vert.spv";
		pipelineSpec.ShaderBinaries[ShaderStage::Fragment] = "Resources/Shaders/PieceShader.frag.spv";
		pipelineSpec.VertexInput = {
			VertexDataType::Float3,
			VertexDataType::Float3,
			VertexDataType::Float3
		};

		m_Pipeline = m_Renderer->CreatePipeline(pipelineSpec);

		m_PieceUniforms.ProjectionMatrix = glm::mat4(1.0f);
		m_PieceUniforms.ViewMatrix = glm::mat4(1.0f);
		m_PieceUniforms.ModelMatrix = glm::translate(glm::mat4(1.0f), {0.0f, 0.1f, 0.0f})
			* glm::scale(glm::mat4(1.0f), glm::vec3(7.0f));

		m_UniformBuffer = m_Renderer->CreateUniformBuffer(sizeof(m_PieceUniforms), &m_PieceUniforms);
		m_Pipeline->WriteDescriptor("Uniforms", m_UniformBuffer);
	}

	PieceRenderer::~PieceRenderer()
	{
	}

	void PieceRenderer::BeginFrame(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix)
	{
		if (projectionMatrix != m_CachedProjectionMatrix || viewMatrix != m_CachedViewMatrix)
		{
			m_CachedProjectionMatrix = projectionMatrix;
			m_CachedViewMatrix = viewMatrix;

			m_PieceUniforms.ProjectionMatrix = m_CachedProjectionMatrix;
			m_PieceUniforms.ViewMatrix = m_CachedViewMatrix;
			m_UniformBuffer->SetData(sizeof(m_PieceUniforms), &m_PieceUniforms);
		}

		m_Renderer->BindPipeline(m_Pipeline);
	}

	void PieceRenderer::EndFrame()
	{
	}

	void PieceRenderer::RenderPiece(const Piece& piece)
	{
		const auto& model = piece.GetModel();

		m_Renderer->BindVertexBuffer(model.VertexBuffer);
		m_Renderer->BindIndexBuffer(model.IndexBuffer);

		m_Renderer->DrawIndexed(model.IndexCount);
	}

}
