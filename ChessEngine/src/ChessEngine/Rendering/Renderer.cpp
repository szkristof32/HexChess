#include "pch.h"
#include "Renderer.h"

#include "ChessEngine/Rendering/RendererContext.h"
#include "ChessEngine/Rendering/RendererBackend.h"
#include "ChessEngine/Rendering/Pipeline.h"

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

	void Renderer::BindPipeline(const std::shared_ptr<Pipeline>& pipeline) const
	{
		m_RendererBackend->BindPipeline(pipeline);
	}

	void Renderer::Draw(uint32_t vertexCount) const
	{
		m_RendererBackend->Draw(vertexCount);
	}

	void Renderer::OnResize(uint32_t width, uint32_t height)
	{
		m_RendererBackend->OnResize(width, height);
	}

}
