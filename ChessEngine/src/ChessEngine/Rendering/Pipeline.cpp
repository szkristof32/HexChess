#include "pch.h"
#include "Pipeline.h"

#include "VulkanUtils.h"

namespace ChessEngine {

	namespace PipelineUtils {

		static VkShaderStageFlagBits ShaderStageToVkStage(ShaderStage stage)
		{
			switch (stage)
			{
				case ShaderStage::Vertex:	return VK_SHADER_STAGE_VERTEX_BIT;
				case ShaderStage::Fragment:	return VK_SHADER_STAGE_FRAGMENT_BIT;
			}

			return (VkShaderStageFlagBits)0;
		}

	}

	Pipeline::Pipeline(const PipelineSpecification& spec, const std::shared_ptr<RendererContext>& context, const std::shared_ptr<RendererBackend>& backend)
		: m_Context(context), m_Backend(backend), m_Spec(spec)
	{
		Create();
	}

	Pipeline::~Pipeline()
	{
		vkDestroyPipelineLayout(m_Context->GetDevice(), m_PipelineLayout, nullptr);
		vkDestroyPipeline(m_Context->GetDevice(), m_Pipeline, nullptr);
	}

	void Pipeline::Create()
	{
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		shaderStages.reserve(m_Spec.ShaderBinaries.size());

		for (const auto& [stage, path] : m_Spec.ShaderBinaries)
		{
			auto vkStage = PipelineUtils::ShaderStageToVkStage(stage);
			VkShaderModule shaderModule = CreateShaderModule(vkStage, path);

			VkPipelineShaderStageCreateInfo& shaderStage = shaderStages.emplace_back();
			shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStage.stage = vkStage;
			shaderStage.module = shaderModule;
			shaderStage.pName = "main";
		}

		VkPipelineVertexInputStateCreateInfo vertexInput{};
		vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInput.vertexAttributeDescriptionCount = 0;
		vertexInput.pVertexAttributeDescriptions = nullptr;
		vertexInput.vertexBindingDescriptionCount = 0;
		vertexInput.pVertexBindingDescriptions = nullptr;

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = false;

		std::array dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR
		};
		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = (uint32_t)dynamicStates.size();
		dynamicState.pDynamicStates = dynamicStates.data();

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasteriser{};
		rasteriser.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasteriser.polygonMode = VK_POLYGON_MODE_FILL;
		rasteriser.lineWidth = 1.0f;
		rasteriser.cullMode = VK_CULL_MODE_BACK_BIT;
		rasteriser.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasteriser.depthBiasEnable = false;
		rasteriser.depthClampEnable = false;
		rasteriser.rasterizerDiscardEnable = false;

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = false;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f;
		multisampling.pSampleMask = nullptr;
		multisampling.alphaToCoverageEnable = false;
		multisampling.alphaToOneEnable = false;

		VkPipelineColorBlendAttachmentState colourBlendAttachment{};
		colourBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colourBlendAttachment.blendEnable = false;
		colourBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colourBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colourBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colourBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colourBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		colourBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colourBlending{};
		colourBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colourBlending.attachmentCount = 1;
		colourBlending.pAttachments = &colourBlendAttachment;
		colourBlending.logicOpEnable = false;

		VkPipelineLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layoutInfo.pushConstantRangeCount = 0;
		layoutInfo.pPushConstantRanges = nullptr;
		layoutInfo.setLayoutCount = 0;
		layoutInfo.pSetLayouts = nullptr;

		VK_CHECK(vkCreatePipelineLayout(m_Context->GetDevice(), &layoutInfo, nullptr, &m_PipelineLayout));

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = (uint32_t)shaderStages.size();
		pipelineInfo.pStages = shaderStages.data();
		pipelineInfo.pVertexInputState = &vertexInput;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasteriser;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pColorBlendState = &colourBlending;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.layout = m_PipelineLayout;
		pipelineInfo.renderPass = m_Backend->GetSwapchainRenderPass();
		pipelineInfo.subpass = 0;

		VK_CHECK(vkCreateGraphicsPipelines(m_Context->GetDevice(), nullptr, 1, &pipelineInfo, nullptr, &m_Pipeline));

		for (const auto& shaderStage : shaderStages)
		{
			vkDestroyShaderModule(m_Context->GetDevice(), shaderStage.module, nullptr);
		}
	}

	VkShaderModule Pipeline::CreateShaderModule(VkShaderStageFlagBits stage, std::string_view path)
	{
		std::ifstream file(path.data(), std::ios::ate | std::ios::binary); // RAII

		size_t fileSize = file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0, std::ios::beg);
		file.read(buffer.data(), fileSize);

		VkShaderModuleCreateInfo moduleInfo{};
		moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		moduleInfo.codeSize = fileSize;
		moduleInfo.pCode = (uint32_t*)buffer.data();

		VkShaderModule shaderModule;
		VK_CHECK(vkCreateShaderModule(m_Context->GetDevice(), &moduleInfo, nullptr, &shaderModule));

		return shaderModule;
	}

}
