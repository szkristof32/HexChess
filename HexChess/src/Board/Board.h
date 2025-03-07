#pragma once

#include "BoardGenerator.h"

#include <ChessEngine/Rendering/Renderer.h>
#include <ChessEngine/Rendering/Pipeline.h>
#include <ChessEngine/Rendering/Buffers.h>
#include <ChessEngine/Rendering/Image.h>

#include <glm/glm.hpp>

namespace HexChess {

	struct BoardUniformBuffer
	{
		glm::mat4 ProjectionMatrix;
		glm::mat4 ViewMatrix;
		glm::mat4 ModelMatrix;
	};

	class Board
	{
	public:
		Board(const std::shared_ptr<ChessEngine::Renderer>& renderer);
		~Board();

		void OnUpdate(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);
	private:
		void GenerateBoard();
		void RenderUI();
	private:
		std::shared_ptr<ChessEngine::Renderer> m_Renderer;
		std::shared_ptr<ChessEngine::Pipeline> m_Pipeline;

		BoardConfig m_BoardConfig;
		BoardGenerator m_Generator;

		std::shared_ptr<ChessEngine::VertexBuffer> m_VertexBuffer;
		std::shared_ptr<ChessEngine::IndexBuffer> m_IndexBuffer;

		std::shared_ptr<ChessEngine::UniformBuffer> m_UniformBuffer;
		BoardUniformBuffer m_BoardUniforms;

		glm::mat4 m_CachedProjectionMatrix;
		glm::mat4 m_CachedViewMatrix;
	};

}
