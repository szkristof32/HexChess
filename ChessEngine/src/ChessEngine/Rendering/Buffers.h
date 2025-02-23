#pragma once

#include <vulkan/vulkan.h>

namespace ChessEngine {

	class RendererContext;
	class RendererBackend;

	class VertexBuffer
	{
	public:
		VertexBuffer(size_t dataSize, const void* data, const std::shared_ptr<RendererContext>& context, const std::shared_ptr<RendererBackend>& backend);
		~VertexBuffer();

		VkBuffer GetBuffer() const { return m_Buffer; }
	private:
		std::shared_ptr<RendererContext> m_Context;
		std::shared_ptr<RendererBackend> m_Backend;

		VkBuffer m_Buffer;
		VkDeviceMemory m_Memory;
	};

	class IndexBuffer
	{
	public:
		IndexBuffer(size_t indexCount, const uint32_t* data, const std::shared_ptr<RendererContext>& context, const std::shared_ptr<RendererBackend>& backend);
		~IndexBuffer();

		VkBuffer GetBuffer() const { return m_Buffer; }
	private:
		std::shared_ptr<RendererContext> m_Context;
		std::shared_ptr<RendererBackend> m_Backend;

		VkBuffer m_Buffer;
		VkDeviceMemory m_Memory;
	};

}
