#pragma once

struct GLFWwindow;

namespace ChessEngine {

	class RendererContext;
	class RendererBackend;

	struct PipelineSpecification;
	class Pipeline;

	class Renderer
	{
	public:
		Renderer(GLFWwindow* windowHandle);
		~Renderer();

		void BeginFrame();
		void EndFrame();

		void WaitIdle() const;

		std::shared_ptr<Pipeline> CreatePipeline(const PipelineSpecification& spec);
		void BindPipeline(const std::shared_ptr<Pipeline>& pipeline) const;
		void Draw(uint32_t vertexCount) const;
	private:
		std::shared_ptr<RendererContext> m_RendererContext;
		std::shared_ptr<RendererBackend> m_RendererBackend;
	};

}
