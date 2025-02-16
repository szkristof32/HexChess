#pragma once

#include "ChessEngine/Rendering/RendererBackend.h"

namespace ChessEngine {

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void BeginFrame();
		void EndFrame();
	private:
		std::unique_ptr<RendererBackend> m_RendererBackend;
	};

}
