#pragma once

#include "ChessEngine/Rendering/RendererContext.h"
#include "ChessEngine/Rendering/RendererBackend.h"

#include <vulkan/vulkan.h>

namespace ChessEngine {

	enum class ShaderStage
	{
		None = 0,
		Vertex, Fragment
	};

	struct PipelineSpecification
	{
		std::unordered_map<ShaderStage, std::string_view> ShaderBinaries;
	};
	
	class Pipeline
	{
	public:
		Pipeline(const PipelineSpecification& spec, const std::shared_ptr<RendererContext>& context, const std::shared_ptr<RendererBackend>& backend);
		~Pipeline();

		VkPipeline GetPipeline() const { return m_Pipeline; }
	private:
		void Create();
		VkShaderModule CreateShaderModule(VkShaderStageFlagBits stage, std::string_view path);
	private:
		std::shared_ptr<RendererContext> m_Context;
		std::shared_ptr<RendererBackend> m_Backend;
		PipelineSpecification m_Spec;

		VkPipeline m_Pipeline;
		VkPipelineLayout m_PipelineLayout;
	};

}
