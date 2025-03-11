#pragma once

#include "Board/Board.h"

#include <ChessEngine/Rendering/Renderer.h>
#include <ChessEngine/Rendering/Pipeline.h>
#include <ChessEngine/Rendering/Buffers.h>

namespace HexChess {

	struct BoardUniformBuffer
	{
		glm::mat4 ProjectionMatrix;
		glm::mat4 ViewMatrix;
		glm::mat4 ModelMatrix;
	};

	class BoardRenderer
	{
	public:
		BoardRenderer(const std::shared_ptr<ChessEngine::Renderer>& renderer);
		~BoardRenderer();

		void BeginFrame(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);
		void EndFrame();

		void RenderBoard(const Board& piece);
	private:
		std::shared_ptr<ChessEngine::Renderer> m_Renderer;

		std::shared_ptr<ChessEngine::Pipeline> m_Pipeline;

		std::shared_ptr<ChessEngine::UniformBuffer> m_UniformBuffer;
		BoardUniformBuffer m_BoardUniforms;

		glm::mat4 m_CachedProjectionMatrix;
		glm::mat4 m_CachedViewMatrix;
	};

}
