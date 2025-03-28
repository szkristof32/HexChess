#include "pch.h"
#include "Pipeline.h"

#include "VulkanUtils.h"

namespace ChessEngine {

	namespace PipelineUtils {

		static ShaderStage VkStageToShaderStage(VkShaderStageFlagBits vkStage)
		{
			ShaderStage stage = ShaderStage::None;

			if (vkStage & VK_SHADER_STAGE_VERTEX_BIT)	stage |= ShaderStage::Vertex;
			if (vkStage & VK_SHADER_STAGE_FRAGMENT_BIT)	stage |= ShaderStage::Fragment;

			return stage;
		}

		static VkShaderStageFlagBits ShaderStageToVkStage(ShaderStage stage)
		{
			VkShaderStageFlagBits vkStage = (VkShaderStageFlagBits)0;

			if (stage & ShaderStage::Vertex)	vkStage |= VK_SHADER_STAGE_VERTEX_BIT;
			if (stage & ShaderStage::Fragment)	vkStage |= VK_SHADER_STAGE_FRAGMENT_BIT;

			return vkStage;
		}

		static VkFormat VertexDataTypeToVkFormat(VertexDataType dataType)
		{
			switch (dataType)
			{
				case VertexDataType::Float:		return VK_FORMAT_R32_SFLOAT;
				case VertexDataType::Float2:	return VK_FORMAT_R32G32_SFLOAT;
				case VertexDataType::Float3:	return VK_FORMAT_R32G32B32_SFLOAT;
				case VertexDataType::Float4:	return VK_FORMAT_R32G32B32A32_SFLOAT;
				case VertexDataType::Int:		return VK_FORMAT_R32_SINT;
				case VertexDataType::Int2:		return VK_FORMAT_R32G32_SINT;
				case VertexDataType::Int3:		return VK_FORMAT_R32G32B32_SINT;
				case VertexDataType::Int4:		return VK_FORMAT_R32G32B32A32_SINT;
				case VertexDataType::Mat3:		return VK_FORMAT_R32G32B32_SFLOAT;
				case VertexDataType::Mat4:		return VK_FORMAT_R32G32B32A32_SFLOAT;
				case VertexDataType::Bool:		return VK_FORMAT_R8_SINT;
			}

			return VK_FORMAT_UNDEFINED;
		}

		static uint32_t VertexDataTypeSize(VertexDataType dataType)
		{
			switch (dataType)
			{
				case VertexDataType::Float:		return 1 * 4;
				case VertexDataType::Float2:	return 2 * 4;
				case VertexDataType::Float3:	return 3 * 4;
				case VertexDataType::Float4:	return 4 * 4;
				case VertexDataType::Int:		return 1 * 4;
				case VertexDataType::Int2:		return 2 * 4;
				case VertexDataType::Int3:		return 3 * 4;
				case VertexDataType::Int4:		return 4 * 4;
				case VertexDataType::Mat3:		return 3 * 4;
				case VertexDataType::Mat4:		return 4 * 4;
				case VertexDataType::Bool:		return 1;
			}

			return 0;
		}

		static VkDescriptorType GetDescriptorType(ShaderResourceType type)
		{
			switch (type)
			{
				case ShaderResourceType::Uniform:	return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				case ShaderResourceType::Sampler:	return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				case ShaderResourceType::Storage:	return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			}

			return (VkDescriptorType)-1;
		}

	}

	Pipeline::Pipeline(const PipelineSpecification& spec, const std::shared_ptr<RendererContext>& context, const std::shared_ptr<RendererBackend>& backend)
		: m_Context(context), m_Backend(backend), m_Spec(spec)
	{
		Create();
	}

	Pipeline::~Pipeline()
	{
		vkDestroyDescriptorPool(m_Context->GetDevice(), m_DescriptorPool, nullptr);
		for (const auto& descriptorSetLayout : m_DescriptorSetLayouts)
		{
			vkDestroyDescriptorSetLayout(m_Context->GetDevice(), descriptorSetLayout, nullptr);
		}

		vkDestroyPipelineLayout(m_Context->GetDevice(), m_PipelineLayout, nullptr);
		vkDestroyPipeline(m_Context->GetDevice(), m_Pipeline, nullptr);
	}

	void Pipeline::WriteDescriptor(std::string_view name, std::weak_ptr<UniformBuffer> uniformBuffer)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffer.lock()->GetBuffer();
		bufferInfo.offset = 0;
		bufferInfo.range = VK_WHOLE_SIZE;

		auto shaderResource = FindShaderResource(name, ShaderResourceType::Uniform);

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = m_DescriptorSets[shaderResource.DescriptorSetIndex];
		descriptorWrite.dstBinding = shaderResource.Binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = shaderResource.DescriptorCount;
		descriptorWrite.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(m_Context->GetDevice(), 1, &descriptorWrite, 0, nullptr);
	}

	void Pipeline::WriteDescriptor(std::string_view name, std::weak_ptr<StorageBuffer> storageBuffer)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = storageBuffer.lock()->GetBuffer();
		bufferInfo.offset = 0;
		bufferInfo.range = VK_WHOLE_SIZE;

		auto shaderResource = FindShaderResource(name, ShaderResourceType::Storage);

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = m_DescriptorSets[shaderResource.DescriptorSetIndex];
		descriptorWrite.dstBinding = shaderResource.Binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		descriptorWrite.descriptorCount = shaderResource.DescriptorCount;
		descriptorWrite.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(m_Context->GetDevice(), 1, &descriptorWrite, 0, nullptr);
	}

	void Pipeline::WriteDescriptor(std::string_view name, std::weak_ptr<Image> image)
	{
		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = image.lock()->GetImageView();
		imageInfo.sampler = image.lock()->GetSampler();

		auto shaderResource = FindShaderResource(name, ShaderResourceType::Sampler);

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = m_DescriptorSets[shaderResource.DescriptorSetIndex];
		descriptorWrite.dstBinding = shaderResource.Binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrite.descriptorCount = shaderResource.DescriptorCount;
		descriptorWrite.pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(m_Context->GetDevice(), 1, &descriptorWrite, 0, nullptr);
	}

	void Pipeline::PushConstants(std::string_view name, size_t dataSize, const void* data)
	{
		for (const auto& pushConstant : m_ReflectionData.PushConstants)
		{
			if (pushConstant.Name == name && pushConstant.Size == dataSize)
			{
				m_Backend->PushConstants(m_PipelineLayout,
					PipelineUtils::ShaderStageToVkStage(pushConstant.Stage),
					pushConstant.Offset,
					pushConstant.Size,
					data);
				return;
			}
		}
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

		std::vector<VkVertexInputAttributeDescription> vertexAttributes;
		vertexAttributes.reserve(m_Spec.VertexInput.size());
		std::vector<VkVertexInputBindingDescription> vertexBindings;
		vertexBindings.reserve(1);

		uint32_t index = 0, offset = 0;
		for (const auto& attribute : m_Spec.VertexInput)
		{
			VkVertexInputAttributeDescription& vertexAttribute = vertexAttributes.emplace_back();
			vertexAttribute.binding = 0;
			vertexAttribute.location = index++;
			vertexAttribute.format = PipelineUtils::VertexDataTypeToVkFormat(attribute);
			vertexAttribute.offset = offset;

			offset += PipelineUtils::VertexDataTypeSize(attribute);
		}

		{
			VkVertexInputBindingDescription& vertexBinding = vertexBindings.emplace_back();
			vertexBinding.binding = 0;
			vertexBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			vertexBinding.stride = offset;
		}

		VkPipelineVertexInputStateCreateInfo vertexInput{};
		vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInput.vertexAttributeDescriptionCount = (uint32_t)vertexAttributes.size();
		vertexInput.pVertexAttributeDescriptions = vertexAttributes.data();
		vertexInput.vertexBindingDescriptionCount = (uint32_t)vertexBindings.size();
		vertexInput.pVertexBindingDescriptions = vertexBindings.data();

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
		rasteriser.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasteriser.depthBiasEnable = false;
		rasteriser.depthClampEnable = false;
		rasteriser.rasterizerDiscardEnable = false;

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = false;
		multisampling.rasterizationSamples = m_Backend->GetMaxMSAASamples();
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

		VkPipelineDepthStencilStateCreateInfo depthStencil{};
		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.depthTestEnable = true;
		depthStencil.depthWriteEnable = true;
		depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
		depthStencil.depthBoundsTestEnable = false;
		depthStencil.stencilTestEnable = false;

		CreateDescriptorSetLayout();
		CreateDescriptorPool();
		CreateDescriptorSets();

		std::vector<VkPushConstantRange> pushConstants;
		pushConstants.reserve(m_ReflectionData.PushConstants.size());
		
		for (const auto& pushConstant : m_ReflectionData.PushConstants)
		{
			VkPushConstantRange& range = pushConstants.emplace_back();
			range.offset = (uint32_t)pushConstant.Offset;
			range.size = (uint32_t)pushConstant.Size;
			range.stageFlags = PipelineUtils::ShaderStageToVkStage(pushConstant.Stage);
		}

		VkPipelineLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layoutInfo.pushConstantRangeCount = (uint32_t)pushConstants.size();
		layoutInfo.pPushConstantRanges = pushConstants.data();
		layoutInfo.setLayoutCount = (uint32_t)m_DescriptorSetLayouts.size();
		layoutInfo.pSetLayouts = m_DescriptorSetLayouts.data();

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
		pipelineInfo.pDepthStencilState = &depthStencil;
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

	void Pipeline::CreateDescriptorSetLayout()
	{
		m_DescriptorSetLayouts.reserve(m_ReflectionData.DescriptorSets.size());

		for (const auto& [index, descriptorSet] : m_ReflectionData.DescriptorSets)
		{
			std::vector<VkDescriptorSetLayoutBinding> bindings;
			bindings.reserve(descriptorSet.Resources.size());

			for (const auto& resource : descriptorSet.Resources)
			{
				VkDescriptorSetLayoutBinding& binding = bindings.emplace_back();
				binding.binding = resource.Binding;
				binding.descriptorType = PipelineUtils::GetDescriptorType(resource.Type);
				binding.descriptorCount = resource.DescriptorCount;
				binding.stageFlags = PipelineUtils::ShaderStageToVkStage(resource.Stage);

				m_DescriptorCount++;
			}

			VkDescriptorSetLayoutCreateInfo setLayoutInfo{};
			setLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			setLayoutInfo.bindingCount = (uint32_t)bindings.size();
			setLayoutInfo.pBindings = bindings.data();

			VkDescriptorSetLayout& layout = m_DescriptorSetLayouts.emplace_back();
			VK_CHECK(vkCreateDescriptorSetLayout(m_Context->GetDevice(), &setLayoutInfo, nullptr, &layout));
		}
	}

	void Pipeline::CreateDescriptorPool()
	{
		std::vector<VkDescriptorPoolSize> poolSizes;
		poolSizes.reserve(m_DescriptorCount);

		for (const auto& [index, descriptorSet] : m_ReflectionData.DescriptorSets)
		{
			for (const auto& resource : descriptorSet.Resources)
			{
				VkDescriptorPoolSize& poolSize = poolSizes.emplace_back();
				poolSize.type = PipelineUtils::GetDescriptorType(resource.Type);
				poolSize.descriptorCount = resource.DescriptorCount;
			}
		}

		VkDescriptorPoolCreateInfo descriptorPoolInfo{};
		descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolInfo.poolSizeCount = (uint32_t)poolSizes.size();
		descriptorPoolInfo.pPoolSizes = poolSizes.data();
		descriptorPoolInfo.maxSets = m_DescriptorCount;

		VK_CHECK(vkCreateDescriptorPool(m_Context->GetDevice(), &descriptorPoolInfo, nullptr, &m_DescriptorPool));
	}

	void Pipeline::CreateDescriptorSets()
	{
		m_DescriptorSets.resize(m_DescriptorSetLayouts.size());

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_DescriptorPool;
		allocInfo.descriptorSetCount = (uint32_t)m_DescriptorSetLayouts.size();
		allocInfo.pSetLayouts = m_DescriptorSetLayouts.data();

		VK_CHECK(vkAllocateDescriptorSets(m_Context->GetDevice(), &allocInfo, m_DescriptorSets.data()));
	}

	ShaderResource Pipeline::FindShaderResource(std::string_view name, ShaderResourceType type)
	{
		for (const auto& [index, descriptorSet] : m_ReflectionData.DescriptorSets)
		{
			for (const auto& resource : descriptorSet.Resources)
			{
				if (resource.Name == name && resource.Type == type)
					return resource;
			}
		}

		return {};
	}

	VkShaderModule Pipeline::CreateShaderModule(VkShaderStageFlagBits stage, std::string_view path)
	{
		std::ifstream file(path.data(), std::ios::ate | std::ios::binary); // RAII

		size_t fileSize = file.tellg();
		std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));

		file.seekg(0, std::ios::beg);
		file.read((char*)buffer.data(), fileSize);

		ReflectShaderStage(buffer, PipelineUtils::VkStageToShaderStage(stage), m_ReflectionData);

		VkShaderModuleCreateInfo moduleInfo{};
		moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		moduleInfo.codeSize = fileSize;
		moduleInfo.pCode = (uint32_t*)buffer.data();

		VkShaderModule shaderModule;
		VK_CHECK(vkCreateShaderModule(m_Context->GetDevice(), &moduleInfo, nullptr, &shaderModule));

		return shaderModule;
	}

}
