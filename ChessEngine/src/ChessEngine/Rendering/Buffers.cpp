#include "pch.h"
#include "Buffers.h"

#include "ChessEngine/Rendering/RendererContext.h"

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

	}

	VertexBuffer::VertexBuffer(size_t dataSize, const void* data, const std::shared_ptr<RendererContext>& context)
		: m_Context(context)
	{
		BufferUtils::CreateBuffer(dataSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, m_Context->GetDevice(), &m_Buffer);
		BufferUtils::AllocateMemory(m_Buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_Context, &m_Memory);

		void* mapped;
		vkMapMemory(m_Context->GetDevice(), m_Memory, 0, dataSize, 0, &mapped);
		memcpy(mapped, data, dataSize);
		vkUnmapMemory(m_Context->GetDevice(), m_Memory);
	}

	VertexBuffer::~VertexBuffer()
	{
		vkFreeMemory(m_Context->GetDevice(), m_Memory, nullptr);
		vkDestroyBuffer(m_Context->GetDevice(), m_Buffer, nullptr);
	}

}
