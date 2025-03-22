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

	struct BoardColouring
	{
		glm::vec2 ExtraColouredCell = glm::vec2(1000.0f);
	};

	class BoardRenderer
	{
	public:
		BoardRenderer(const std::shared_ptr<ChessEngine::Renderer>& renderer);
		~BoardRenderer();

		void BeginFrame(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);
		void EndFrame();

		void RenderBoard(const Board& piece);
		void SetExtraColouredCell(const glm::vec2& position) { m_BoardColouring.ExtraColouredCell = position; }
	private:
		std::shared_ptr<ChessEngine::Renderer> m_Renderer;

		std::shared_ptr<ChessEngine::Pipeline> m_Pipeline;

		std::shared_ptr<ChessEngine::UniformBuffer> m_UniformBuffer;
		BoardUniformBuffer m_BoardUniforms;

		std::shared_ptr<ChessEngine::UniformBuffer> m_UniformBufferColouring;
		BoardColouring m_BoardColouring{};
		BoardColouring m_BoardColouringPrevFrame{};

		glm::mat4 m_CachedProjectionMatrix;
		glm::mat4 m_CachedViewMatrix;
	};

}
