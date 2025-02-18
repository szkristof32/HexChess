#pragma once

#include "ChessEngine/Rendering/RendererContext.h"

namespace ChessEngine {

	class Renderer
	{
	public:
		Renderer(GLFWwindow* windowHandle);
		~Renderer();

		void BeginFrame();
		void EndFrame();
	private:
		std::unique_ptr<RendererContext> m_RendererContext;
	};

}
