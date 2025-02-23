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

	enum class VertexDataType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Int, Int2, Int3, Int4,
		Mat3, Mat4,
		Bool
	};

	struct PipelineSpecification
	{
		std::unordered_map<ShaderStage, std::string_view> ShaderBinaries;
		std::vector<VertexDataType> VertexInput;
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
