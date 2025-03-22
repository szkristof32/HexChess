#pragma once

#include "Board/Piece.h"

#include <ChessEngine/Rendering/Renderer.h>
#include <ChessEngine/Rendering/Pipeline.h>
#include <ChessEngine/Rendering/Buffers.h>

namespace HexChess {

	struct PieceUniformBuffer
	{
		glm::mat4 ProjectionMatrix;
		glm::mat4 ViewMatrix;
	};

	struct PiecePushConstant
	{
		glm::mat4 ModelMatrix;
		glm::vec3 Colour;
		float IsSelected = 0.0f;
	};

	class PieceRenderer
	{
	public:
		PieceRenderer(const std::shared_ptr<ChessEngine::Renderer>& renderer);
		~PieceRenderer();

		void BeginFrame(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);
		void EndFrame();
		
		void RenderPiece(const Piece& piece, const glm::vec3& position = glm::vec3(0.0f));
	private:
		void RenderUI();
	private:
		std::shared_ptr<ChessEngine::Renderer> m_Renderer;

		std::shared_ptr<ChessEngine::Pipeline> m_Pipeline;
		
		std::shared_ptr<ChessEngine::UniformBuffer> m_UniformBuffer;
		PieceUniformBuffer m_PieceUniforms;
		PiecePushConstant m_PiecePushConstant;

		glm::mat4 m_CachedProjectionMatrix;
		glm::mat4 m_CachedViewMatrix;

		glm::mat4 m_RotationMatrix;
		glm::mat4 m_ScaleMatrix;

		glm::vec3 m_LightColour = glm::vec3(0.95f);
		glm::vec3 m_DarkColour = glm::vec3(0.15f);
		float m_PieceSize = 2.0f;
	};

}
