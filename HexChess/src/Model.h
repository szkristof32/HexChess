#pragma once

#include <ChessEngine/Rendering/Buffers.h>

namespace HexChess {

	struct Model
	{
		std::shared_ptr<ChessEngine::VertexBuffer> VertexBuffer;
		std::shared_ptr<ChessEngine::IndexBuffer> IndexBuffer;

		uint32_t IndexCount;
	};

}
