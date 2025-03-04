#pragma once

#include "ChessEngine/Rendering/RendererContext.h"
#include "ChessEngine/Rendering/RendererBackend.h"
#include "ChessEngine/Rendering/Pipeline.h"
#include "ChessEngine/Rendering/Buffers.h"
#include "ChessEngine/Rendering/Image.h"

struct GLFWwindow;

namespace ChessEngine {

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
		std::shared_ptr<UniformBuffer> CreateUniformBuffer(size_t dataSize, const void* data);

		std::shared_ptr<Image> CreateImage(const std::filesystem::path& filepath);

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
