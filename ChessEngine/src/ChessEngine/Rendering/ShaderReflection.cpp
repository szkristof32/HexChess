#include "pch.h"
#include "ShaderReflection.h"

#include <spirv_cross/spirv_cross.hpp>

namespace ChessEngine {

	void ReflectShaderStage(const std::vector<uint32_t>& code, ShaderStage stage, std::unordered_map<uint32_t, DescriptorSet>& reflectionData)
	{
		spirv_cross::Compiler compiler(code);
		auto resources = compiler.get_shader_resources();

		for (const auto& resource : resources.uniform_buffers)
		{
			auto activeBuffers = compiler.get_active_buffer_ranges(resource.id);
			if (activeBuffers.empty())
				continue;

			const auto& name = resource.name;
			auto& bufferType = compiler.get_type(resource.base_type_id);
			int memberCount = (uint32_t)bufferType.member_types.size();
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			uint32_t descriptorSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
			auto size = (uint32_t)compiler.get_declared_struct_size(bufferType);

			DescriptorSet& shaderDescriptorSet = reflectionData[descriptorSet];

			ShaderResource& uniformBuffer = shaderDescriptorSet.Resources.emplace_back();
			uniformBuffer.Type = ShaderResourceType::Uniform;
			uniformBuffer.Binding = binding;
			uniformBuffer.Size = size;
			uniformBuffer.Name = std::move(name);
			uniformBuffer.Stage = stage;
			uniformBuffer.DescriptorCount = 1;
		}
	}

}
