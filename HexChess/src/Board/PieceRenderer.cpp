#include <pch.h>
#include "PieceRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace HexChess {

	using ChessEngine::ShaderStage;
	using ChessEngine::VertexDataType;

	namespace PieceUtils {

		static glm::vec3 PieceTypeColour(PieceType type)
		{
			if (type & PieceType::White)	return glm::vec3(0.95f);
			if (type & PieceType::Black)	return glm::vec3(0.15f);

			return { 1.0f, 0.0f, 1.0f };
		}

	}

	PieceRenderer::PieceRenderer(const std::shared_ptr<ChessEngine::Renderer>& renderer)
		: m_Renderer(renderer)
	{
		ChessEngine::PipelineSpecification pipelineSpec{};
		pipelineSpec.ShaderBinaries[ShaderStage::Vertex] = "Resources/Shaders/PieceShader.vert.spv";
		pipelineSpec.ShaderBinaries[ShaderStage::Fragment] = "Resources/Shaders/PieceShader.frag.spv";
		pipelineSpec.VertexInput = {
			VertexDataType::Float3,
			VertexDataType::Float3
		};

		m_Pipeline = m_Renderer->CreatePipeline(pipelineSpec);

		m_PieceUniforms.ProjectionMatrix = glm::mat4(1.0f);
		m_PieceUniforms.ViewMatrix = glm::mat4(1.0f);

		m_PiecePushConstant.ModelMatrix = glm::mat4(1.0f);
		m_ScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));

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

	void PieceRenderer::RenderPiece(const Piece& piece, const glm::vec3& position)
	{
		m_PiecePushConstant.ModelMatrix = glm::translate(glm::mat4(1.0f), position) * m_ScaleMatrix;
		m_PiecePushConstant.Colour = PieceUtils::PieceTypeColour(piece.GetPieceType());
		m_Pipeline->PushConstants("PieceConstants", sizeof(m_PiecePushConstant), &m_PiecePushConstant);

		const auto& model = piece.GetModel();

		m_Renderer->BindVertexBuffer(model.VertexBuffer);
		m_Renderer->BindIndexBuffer(model.IndexBuffer);

		m_Renderer->DrawIndexed(model.IndexCount);
	}

}
