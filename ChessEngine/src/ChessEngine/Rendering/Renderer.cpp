#include "pch.h"
#include "Renderer.h"

#include "ChessEngine/Rendering/RendererContext.h"
#include "ChessEngine/Rendering/RendererBackend.h"
#include "ChessEngine/Rendering/Pipeline.h"
#include "ChessEngine/Rendering/Buffers.h"

namespace ChessEngine {

	Renderer::Renderer(GLFWwindow* windowHandle)
	{
		m_RendererContext = std::make_shared<RendererContext>(windowHandle);
		m_RendererBackend = std::make_shared<RendererBackend>(m_RendererContext, windowHandle);
	}

	Renderer::~Renderer()
	{
		m_RendererContext->WaitIdle();
	}

	bool Renderer::BeginFrame()
	{
		m_RendererContext->BeginFrame();
		bool result = m_RendererBackend->BeginFrame();

		return result;
	}

	bool Renderer::EndFrame()
	{
		bool result = m_RendererBackend->EndFrame();
		m_RendererContext->EndFrame();

		return result;
	}

	void Renderer::WaitIdle() const
	{
		m_RendererContext->WaitIdle();
	}

	std::shared_ptr<Pipeline> Renderer::CreatePipeline(const PipelineSpecification& spec)
	{
		return std::make_shared<Pipeline>(spec, m_RendererContext, m_RendererBackend);
	}

	std::shared_ptr<VertexBuffer> Renderer::CreateVertexBuffer(size_t dataSize, const void* data)
	{
		return std::make_shared<VertexBuffer>(dataSize, data, m_RendererContext, m_RendererBackend);
	}

	std::shared_ptr<IndexBuffer> Renderer::CreateIndexBuffer(size_t indexCount, const uint32_t* data)
	{
		return std::make_shared<IndexBuffer>(indexCount, data, m_RendererContext, m_RendererBackend);
	}

	std::shared_ptr<UniformBuffer> Renderer::CreateUniformBuffer(size_t dataSize, const void* data)
	{
		return std::make_shared<UniformBuffer>(dataSize, data, m_RendererContext, m_RendererBackend);
	}

	void Renderer::BindPipeline(const std::shared_ptr<Pipeline>& pipeline) const
	{
		m_RendererBackend->BindPipeline(pipeline);
	}

	void Renderer::BindVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) const
	{
		m_RendererBackend->BindVertexBuffer(vertexBuffer);
	}

	void Renderer::BindIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) const
	{
		m_RendererBackend->BindIndexBuffer(indexBuffer);
	}

	void Renderer::Draw(uint32_t vertexCount) const
	{
		m_RendererBackend->Draw(vertexCount);
	}

	void Renderer::DrawIndexed(uint32_t indexCount) const
	{
		m_RendererBackend->DrawIndexed(indexCount);
	}

	void Renderer::OnResize(uint32_t width, uint32_t height)
	{
		m_RendererBackend->OnResize(width, height);
	}

}
