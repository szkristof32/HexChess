#pragma once

#include <vulkan/vulkan.h>

namespace ChessEngine {

	class RendererContext;

	class VertexBuffer
	{
	public:
		VertexBuffer(size_t dataSize, const void* data, const std::shared_ptr<RendererContext>& context);
		~VertexBuffer();

		VkBuffer GetBuffer() const { return m_Buffer; }
	private:
		std::shared_ptr<RendererContext> m_Context;

		VkBuffer m_Buffer;
		VkDeviceMemory m_Memory;
	};

}
