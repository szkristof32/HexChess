#pragma once

struct GLFWwindow;

namespace ChessEngine {

	class RendererContext;
	class RendererBackend;

	struct PipelineSpecification;
	class Pipeline;
	class VertexBuffer;
	class IndexBuffer;

	class Renderer
	{
	public:
		Renderer(GLFWwindow* windowHandle);
		~Renderer();

		bool BeginFrame();
		bool EndFrame();

		void WaitIdle() const;

		std::shared_ptr<Pipeline> CreatePipeline(const PipelineSpecification& spec);
		std::shared_ptr<VertexBuffer> CreateVertexBuffer(size_t dataSize, const void* data);
		std::shared_ptr<IndexBuffer> CreateIndexBuffer(size_t indexCount, const uint32_t* data);

		void BindPipeline(const std::shared_ptr<Pipeline>& pipeline) const;
		void BindVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) const;
		void BindIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) const;

		void Draw(uint32_t vertexCount) const;
		void DrawIndexed(uint32_t indexCount) const;
	public:
		void OnResize(uint32_t width, uint32_t height);
	private:
		std::shared_ptr<RendererContext> m_RendererContext;
		std::shared_ptr<RendererBackend> m_RendererBackend;
	};

}
