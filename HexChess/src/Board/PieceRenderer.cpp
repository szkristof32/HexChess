#include <pch.h>
#include "PieceRenderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

namespace HexChess {

	using ChessEngine::ShaderStage;
	using ChessEngine::VertexDataType;

	namespace PieceUtils {

		static bool IsBlackPiece(PieceType type)
		{
			return (bool)(type & PieceType::Black);
		}

		static glm::vec3 PieceTypeColour(PieceType type, const glm::vec3& lightColour, const glm::vec3& darkColour)
		{
			return IsBlackPiece(type) ? darkColour : lightColour;
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
		m_RotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), { 0.0f, 1.0f, 0.0f });
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
		RenderUI();
	}

	void PieceRenderer::RenderPiece(const Piece& piece, const glm::vec3& position)
	{
		auto pieceType = piece.GetPieceType();

		m_PiecePushConstant.ModelMatrix = glm::translate(glm::mat4(1.0f), position)
			* (PieceUtils::IsBlackPiece(pieceType) ? m_RotationMatrix : glm::mat4(1.0f))
			* m_ScaleMatrix;
		m_PiecePushConstant.Colour = PieceUtils::PieceTypeColour(pieceType, m_LightColour, m_DarkColour);
		m_PiecePushConstant.IsSelected = (float)piece.IsSelected();
		m_Pipeline->PushConstants("PieceConstants", sizeof(m_PiecePushConstant), &m_PiecePushConstant);

		const auto& model = piece.GetModel();

		m_Renderer->BindVertexBuffer(model.VertexBuffer);
		m_Renderer->BindIndexBuffer(model.IndexBuffer);

		m_Renderer->DrawIndexed(model.IndexCount);
	}

	void PieceRenderer::RenderUI()
	{
		ImGui::Begin("Piece configuration");

		ImGui::ColorEdit3("Light colour", &m_LightColour.r);
		ImGui::ColorEdit3("Dark colour", &m_DarkColour.r);

		ImGui::Separator();

		bool modified = ImGui::DragFloat("Piece size", &m_PieceSize, 0.01f, 0.0f, 100.0f);
		if (modified)
		{
			m_ScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(m_PieceSize));
		}

		ImGui::End();
	}

}
