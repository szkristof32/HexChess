#pragma once

#include <ChessEngine/Rendering/Renderer.h>
#include <ChessEngine/Rendering/Pipeline.h>
#include <ChessEngine/Rendering/Buffers.h>

#include <glm/glm.hpp>

namespace HexChess {

	struct BoardVertex
	{
		glm::vec3 Position;
		glm::vec3 Colour;
	};

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

		void OnUpdate();
	public:
		void OnResize(uint32_t width, uint32_t height);
	private:
		std::shared_ptr<ChessEngine::Renderer> m_Renderer;
		std::shared_ptr<ChessEngine::Pipeline> m_BoardPipeline;

		std::shared_ptr<ChessEngine::VertexBuffer> m_BoardVertexBuffer;
		std::shared_ptr<ChessEngine::IndexBuffer> m_BoardIndexBuffer;

		std::shared_ptr<ChessEngine::UniformBuffer> m_BoardUniformBuffer;
		BoardUniformBuffer m_BoardUniforms;
		uint32_t m_CurrentWidth = 1280, m_CurrentHeight = 720;
	};

}
