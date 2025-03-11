#pragma once

#include "Model.h"
#include "Board/BoardGenerator.h"

#include <ChessEngine/Rendering/Renderer.h>
#include <ChessEngine/Rendering/Buffers.h>

#include <glm/glm.hpp>

namespace HexChess {

	class Board
	{
	public:
		Board(const std::shared_ptr<ChessEngine::Renderer>& renderer);
		~Board();

		void OnUpdate();

		const Model& GetModel() const { return m_Model; }
	private:
		void GenerateBoard();
		void RenderUI();
	private:
		std::shared_ptr<ChessEngine::Renderer> m_Renderer;

		BoardConfig m_BoardConfig;
		BoardGenerator m_Generator;

		Model m_Model;
	};

}
