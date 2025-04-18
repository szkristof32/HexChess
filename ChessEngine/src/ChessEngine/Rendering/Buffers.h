#pragma once

#include <vulkan/vulkan.h>

namespace ChessEngine {

	class RendererContext;
	class RendererBackend;

	namespace BufferUtils {

		void CreateBuffer(size_t dataSize, VkBufferUsageFlags usage, VkDevice device, VkBuffer* outBuffer);
		void AllocateMemory(VkBuffer buffer, VkMemoryPropertyFlags memoryProperties, std::weak_ptr<::ChessEngine::RendererContext> context, VkDeviceMemory* outMemory);
		void CopyFromBuffer(VkBuffer source, VkBuffer destination, VkDeviceSize size, VkCommandBuffer commandBuffer);

	}

	class VertexBuffer
	{
	public:
		VertexBuffer(size_t dataSize, const void* data, const std::shared_ptr<RendererContext>& context, const std::shared_ptr<RendererBackend>& backend);
		~VertexBuffer();

		VkBuffer GetBuffer() const { return m_Buffer; }

		void SetData(size_t dataSize, const void* data);
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

		void SetData(size_t indexCount, const uint32_t* data);
	private:
		std::shared_ptr<RendererContext> m_Context;
		std::shared_ptr<RendererBackend> m_Backend;

		VkBuffer m_Buffer;
		VkDeviceMemory m_Memory;
	};

	class UniformBuffer
	{
	public:
		UniformBuffer(size_t dataSize, const void* data, const std::shared_ptr<RendererContext>& context, const std::shared_ptr<RendererBackend>& backend);
		~UniformBuffer();

		VkBuffer GetBuffer() const { return m_Buffer; }

		void SetData(size_t dataSize, const void* data);
	private:
		std::shared_ptr<RendererContext> m_Context;
		std::shared_ptr<RendererBackend> m_Backend;

		VkBuffer m_Buffer;
		VkDeviceMemory m_Memory;
	};

	class StorageBuffer
	{
	public:
		StorageBuffer(size_t dataSize, const void* data, const std::shared_ptr<RendererContext>& context, const std::shared_ptr<RendererBackend>& backend);
		~StorageBuffer();

		VkBuffer GetBuffer() const { return m_Buffer; }

		void SetData(size_t dataSize, const void* data);
	private:
		std::shared_ptr<RendererContext> m_Context;
		std::shared_ptr<RendererBackend> m_Backend;

		VkBuffer m_Buffer;
		VkDeviceMemory m_Memory;
	};

}
