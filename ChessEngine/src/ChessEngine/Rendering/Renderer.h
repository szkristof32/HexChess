#pragma once

struct GLFWwindow;

namespace ChessEngine {

	class RendererContext;
	class RendererBackend;

	class Renderer
	{
	public:
		Renderer(GLFWwindow* windowHandle);
		~Renderer();

		void BeginFrame();
		void EndFrame();
	private:
		std::shared_ptr<RendererContext> m_RendererContext;
		std::unique_ptr<RendererBackend> m_RendererBackend;
	};

}
