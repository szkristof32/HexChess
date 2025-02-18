#include "pch.h"
#include "Renderer.h"

namespace ChessEngine {

	Renderer::Renderer(GLFWwindow* windowHandle)
	{
		m_RendererContext = std::make_unique<RendererContext>(windowHandle);
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

}
