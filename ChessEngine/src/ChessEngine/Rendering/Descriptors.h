#pragma once

namespace ChessEngine {

	enum class ShaderResourceType
	{
		None = 0,
		Uniform
	};

	enum class ShaderStage
	{
		None = 0,
		Vertex, Fragment
	};

	struct ShaderResource
	{
		std::string_view Name;
		uint32_t Binding;
		uint32_t DescriptorCount;
		ShaderResourceType Type;
		ShaderStage Stage;
	};

	struct DescriptorSet
	{
		std::vector<ShaderResource> Resources;
	};

}
