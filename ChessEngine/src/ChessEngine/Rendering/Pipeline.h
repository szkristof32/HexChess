#pragma once

#include "ChessEngine/Rendering/ShaderReflection.h"

#include "ChessEngine/Rendering/Buffers.h"
#include "ChessEngine/Rendering/Image.h"
#include "ChessEngine/Rendering/RendererContext.h"
#include "ChessEngine/Rendering/RendererBackend.h"

#include <vulkan/vulkan.h>

namespace ChessEngine {

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

		void WriteDescriptor(std::string_view name, std::weak_ptr<UniformBuffer> uniformBuffer);
		void WriteDescriptor(std::string_view name, std::weak_ptr<StorageBuffer> storageBuffer);
		void WriteDescriptor(std::string_view name, std::weak_ptr<Image> image);
		void PushConstants(std::string_view name, size_t dataSize, const void* data);

		VkPipeline GetPipeline() const { return m_Pipeline; }
		VkPipelineLayout GetPipelineLayout() const { return m_PipelineLayout; }
		const std::vector<VkDescriptorSet>& GetDescriptorSets() const { return m_DescriptorSets; }
	private:
		void Create();
		void CreateDescriptorSetLayout();
		void CreateDescriptorPool();
		void CreateDescriptorSets();

		ShaderResource FindShaderResource(std::string_view name, ShaderResourceType type);

		VkShaderModule CreateShaderModule(VkShaderStageFlagBits stage, std::string_view path);
	private:
		std::shared_ptr<RendererContext> m_Context;
		std::shared_ptr<RendererBackend> m_Backend;
		PipelineSpecification m_Spec;

		VkPipeline m_Pipeline;
		VkPipelineLayout m_PipelineLayout;

		ShaderReflection m_ReflectionData;

		VkDescriptorPool m_DescriptorPool;
		uint32_t m_DescriptorCount = 0;
		std::vector<VkDescriptorSetLayout> m_DescriptorSetLayouts;
		std::vector<VkDescriptorSet> m_DescriptorSets;
	};

}
