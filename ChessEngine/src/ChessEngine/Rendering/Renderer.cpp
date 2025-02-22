#include "pch.h"
#include "Renderer.h"

#include "ChessEngine/Rendering/RendererContext.h"
#include "ChessEngine/Rendering/RendererBackend.h"
#include "ChessEngine/Rendering/Pipeline.h"

namespace ChessEngine {

	static std::shared_ptr<Pipeline> s_TempPipeline;

	Renderer::Renderer(GLFWwindow* windowHandle)
	{
		m_RendererContext = std::make_shared<RendererContext>(windowHandle);
		m_RendererBackend = std::make_shared<RendererBackend>(m_RendererContext, windowHandle);

		PipelineSpecification tempPipelineSpec{};
		tempPipelineSpec.ShaderBinaries[ShaderStage::Vertex] = "Resources/Shaders/BoardShader.vert.spv";
		tempPipelineSpec.ShaderBinaries[ShaderStage::Fragment] = "Resources/Shaders/BoardShader.frag.spv";

		s_TempPipeline = CreatePipeline(tempPipelineSpec);
	}

	Renderer::~Renderer()
	{
		m_RendererContext->WaitIdle();

		s_TempPipeline.reset();
	}

	void Renderer::BeginFrame()
	{
		m_RendererContext->BeginFrame();
		m_RendererBackend->BeginFrame();

		m_RendererBackend->BindPipeline(s_TempPipeline);
		m_RendererBackend->Draw(3);
	}

	void Renderer::EndFrame()
	{
		m_RendererBackend->EndFrame();
		m_RendererContext->EndFrame();
	}

	std::shared_ptr<Pipeline> Renderer::CreatePipeline(const PipelineSpecification& spec)
	{
		return std::make_shared<Pipeline>(spec, m_RendererContext, m_RendererBackend);
	}

}
