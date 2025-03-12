#pragma once

#include "ChessEngine/Utils/EnumUtils.h"

namespace ChessEngine {

	enum class ShaderResourceType
	{
		None = 0,
		Uniform, Sampler
	};

	enum class ShaderStage
	{
		None = 0,
		Vertex, Fragment
	};

	struct PushConstantRange
	{
		std::string Name;
		size_t Offset = 0;
		size_t Size;
		ShaderStage Stage;
	};

	struct ShaderResource
	{
		std::string Name;

		uint32_t Binding;
		uint32_t DescriptorCount;
		uint32_t Size;
		uint32_t DescriptorSetIndex;

		ShaderResourceType Type;
		ShaderStage Stage;
	};

	struct DescriptorSet
	{
		std::vector<ShaderResource> Resources;
	};

	struct ShaderReflection
	{
		std::unordered_map<uint32_t, DescriptorSet> DescriptorSets;
		std::vector<PushConstantRange> PushConstants;
	};

	void ReflectShaderStage(const std::vector<uint32_t>& code, ShaderStage stage, ShaderReflection& reflectionData);

}
