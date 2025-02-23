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

	class Board
	{
	public:
		Board(const std::shared_ptr<ChessEngine::Renderer>& renderer);
		~Board();

		void OnUpdate();
	private:
		std::shared_ptr<ChessEngine::Renderer> m_Renderer;
		std::shared_ptr<ChessEngine::Pipeline> m_BoardPipeline;

		std::shared_ptr<ChessEngine::VertexBuffer> m_BoardVertexBuffer;
		std::shared_ptr<ChessEngine::IndexBuffer> m_BoardIndexBuffer;
	};

}
