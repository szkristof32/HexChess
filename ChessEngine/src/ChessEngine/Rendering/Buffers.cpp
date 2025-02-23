#include "pch.h"
#include "Buffers.h"

#include "ChessEngine/Rendering/RendererContext.h"
#include "ChessEngine/Rendering/RendererBackend.h"

#include "VulkanUtils.h"

namespace ChessEngine {

	namespace BufferUtils {

		static void CreateBuffer(size_t dataSize, VkBufferUsageFlags usage, VkDevice device, VkBuffer* outBuffer)
		{
			VkBufferCreateInfo bufferInfo{};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size = dataSize;
			bufferInfo.usage = usage;
			bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			VK_CHECK(vkCreateBuffer(device, &bufferInfo, nullptr, outBuffer));
		}

		static void AllocateMemory(VkBuffer buffer, VkMemoryPropertyFlags memoryProperties, std::weak_ptr<RendererContext> context, VkDeviceMemory* outMemory)
		{
			auto ctx = context.lock();

			VkMemoryRequirements memoryRequirements;
			vkGetBufferMemoryRequirements(ctx->GetDevice(), buffer, &memoryRequirements);

			VkMemoryAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memoryRequirements.size;
			allocInfo.memoryTypeIndex = ctx->FindMemoryType(memoryRequirements.memoryTypeBits, memoryProperties);

			VK_CHECK(vkAllocateMemory(ctx->GetDevice(), &allocInfo, nullptr, outMemory));
			VK_CHECK(vkBindBufferMemory(ctx->GetDevice(), buffer, *outMemory, 0));
		}

		static void CopyBuffer(VkBuffer source, VkBuffer destination, VkDeviceSize size, VkCommandBuffer commandBuffer)
		{
			VkBufferCopy bufferCopy{};
			bufferCopy.srcOffset = 0;
			bufferCopy.dstOffset = 0;
			bufferCopy.size = size;

			vkCmdCopyBuffer(commandBuffer, source, destination, 1, &bufferCopy);
		}

	}

	VertexBuffer::VertexBuffer(size_t dataSize, const void* data, const std::shared_ptr<RendererContext>& context, const std::shared_ptr<RendererBackend>& backend)
		: m_Context(context), m_Backend(backend)
	{
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		BufferUtils::CreateBuffer(dataSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, m_Context->GetDevice(), &stagingBuffer);
		BufferUtils::AllocateMemory(stagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_Context, &stagingBufferMemory);

		BufferUtils::CreateBuffer(dataSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, m_Context->GetDevice(), &m_Buffer);
		BufferUtils::AllocateMemory(m_Buffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Context, &m_Memory);

		void* mapped;
		vkMapMemory(m_Context->GetDevice(), stagingBufferMemory, 0, dataSize, 0, &mapped);
		memcpy(mapped, data, dataSize);
		vkUnmapMemory(m_Context->GetDevice(), stagingBufferMemory);

		VkCommandBuffer commandBuffer = m_Backend->AllocateNewCommandBuffer();
		BufferUtils::CopyBuffer(stagingBuffer, m_Buffer, dataSize, commandBuffer);
		m_Backend->SubmitCommandBuffer(commandBuffer);

		vkFreeMemory(m_Context->GetDevice(), stagingBufferMemory, nullptr);
		vkDestroyBuffer(m_Context->GetDevice(), stagingBuffer, nullptr);
	}

	VertexBuffer::~VertexBuffer()
	{
		vkFreeMemory(m_Context->GetDevice(), m_Memory, nullptr);
		vkDestroyBuffer(m_Context->GetDevice(), m_Buffer, nullptr);
	}

	IndexBuffer::IndexBuffer(size_t indexCount, const uint32_t* data, const std::shared_ptr<RendererContext>& context, const std::shared_ptr<RendererBackend>& backend)
		: m_Context(context), m_Backend(backend)
	{
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		BufferUtils::CreateBuffer(indexCount * sizeof(uint32_t), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, m_Context->GetDevice(), &stagingBuffer);
		BufferUtils::AllocateMemory(stagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_Context, &stagingBufferMemory);

		BufferUtils::CreateBuffer(indexCount * sizeof(uint32_t), VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, m_Context->GetDevice(), &m_Buffer);
		BufferUtils::AllocateMemory(m_Buffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Context, &m_Memory);

		void* mapped;
		vkMapMemory(m_Context->GetDevice(), stagingBufferMemory, 0, indexCount * sizeof(uint32_t), 0, &mapped);
		memcpy(mapped, data, indexCount * sizeof(uint32_t));
		vkUnmapMemory(m_Context->GetDevice(), stagingBufferMemory);

		VkCommandBuffer commandBuffer = m_Backend->AllocateNewCommandBuffer();
		BufferUtils::CopyBuffer(stagingBuffer, m_Buffer, indexCount * sizeof(uint32_t), commandBuffer);
		m_Backend->SubmitCommandBuffer(commandBuffer);

		vkFreeMemory(m_Context->GetDevice(), stagingBufferMemory, nullptr);
		vkDestroyBuffer(m_Context->GetDevice(), stagingBuffer, nullptr);
	}

	IndexBuffer::~IndexBuffer()
	{
		vkFreeMemory(m_Context->GetDevice(), m_Memory, nullptr);
		vkDestroyBuffer(m_Context->GetDevice(), m_Buffer, nullptr);
	}

}
