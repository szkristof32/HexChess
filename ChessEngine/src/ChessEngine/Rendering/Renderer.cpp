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
	}

	void Renderer::BeginFrame()
	{
		m_RendererContext->BeginFrame();
	}

	void Renderer::EndFrame()
	{
		m_RendererContext->EndFrame();
	}

	std::shared_ptr<Pipeline> Renderer::CreatePipeline(const PipelineSpecification& spec)
	{
		return std::make_shared<Pipeline>(spec, m_RendererContext, m_RendererBackend);
	}

}
