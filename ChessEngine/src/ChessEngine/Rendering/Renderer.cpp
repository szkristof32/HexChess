#include "pch.h"
#include "Renderer.h"

namespace ChessEngine {

	Renderer::Renderer()
	{
		m_RendererBackend = std::make_unique<RendererBackend>();
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::BeginFrame()
	{
		m_RendererBackend->BeginFrame();
	}

	void Renderer::EndFrame()
	{
		m_RendererBackend->EndFrame();
	}

}
