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

	struct BoardMarking
	{
		alignas(16) glm::vec2 SelectedCell;
		alignas(16) glm::vec4 MoveCells[21];
	};

	class BoardRenderer
	{
	public:
		BoardRenderer(const std::shared_ptr<ChessEngine::Renderer>& renderer);
		~BoardRenderer();

		void BeginFrame(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);
		void EndFrame();

		void RenderBoard(const Board& board);
		void SetSelectedCell(const glm::vec2& position) { m_BoardMarking.SelectedCell = position; }
	private:
		std::shared_ptr<ChessEngine::Renderer> m_Renderer;

		std::shared_ptr<ChessEngine::Pipeline> m_Pipeline;

		std::shared_ptr<ChessEngine::UniformBuffer> m_UniformBuffer;
		BoardUniformBuffer m_BoardUniforms;

		std::shared_ptr<ChessEngine::StorageBuffer> m_StorageBufferMarking;
		BoardMarking m_BoardMarking{};

		glm::mat4 m_CachedProjectionMatrix;
		glm::mat4 m_CachedViewMatrix;
	};

}
