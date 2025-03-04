#include "pch.h"
#include "Image.h"

#include "ChessEngine/Rendering/Buffers.h"

#include "VulkanUtils.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace ChessEngine {

	namespace ImageUtils {

		void AllocateMemory(VkImage image, VkMemoryPropertyFlags memoryProperties, std::weak_ptr<::ChessEngine::RendererContext> context, VkDeviceMemory* outMemory)
		{
			auto ctx = context.lock();

			VkMemoryRequirements memoryRequirements;
			vkGetImageMemoryRequirements(ctx->GetDevice(), image, &memoryRequirements);

			VkMemoryAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memoryRequirements.size;
			allocInfo.memoryTypeIndex = ctx->FindMemoryType(memoryRequirements.memoryTypeBits, memoryProperties);

			VK_CHECK(vkAllocateMemory(ctx->GetDevice(), &allocInfo, nullptr, outMemory));
			VK_CHECK(vkBindImageMemory(ctx->GetDevice(), image, *outMemory, 0));
		}

		void CopyFromBuffer(VkBuffer source, VkImage destination, uint32_t width, uint32_t height, VkCommandBuffer commandBuffer)
		{
			VkBufferImageCopy imageCopy{};
			imageCopy.bufferOffset = 0;
			imageCopy.bufferRowLength = 0;
			imageCopy.bufferImageHeight = 0;
			imageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageCopy.imageSubresource.mipLevel = 0;
			imageCopy.imageSubresource.baseArrayLayer = 0;
			imageCopy.imageSubresource.layerCount = 1;
			imageCopy.imageOffset = { 0, 0 };
			imageCopy.imageExtent = { width, height, 1 };

			vkCmdCopyBufferToImage(commandBuffer, source, destination, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageCopy);
		}

		void TransitionLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkCommandBuffer commandBuffer)
		{
			VkImageMemoryBarrier barrier{};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.image = image;
			barrier.oldLayout = oldLayout;
			barrier.newLayout = newLayout;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = 1;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = 1;
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = 0;

			VkPipelineStageFlags sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			VkPipelineStageFlags destinationStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

			if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
			{
				barrier.srcAccessMask = 0;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

				sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
				destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			}
			else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
			{
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			}

			vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
		}

	}

	Image::Image(const std::filesystem::path& filepath, const std::shared_ptr<RendererContext>& context, const std::shared_ptr<RendererBackend>& backend)
		: m_Context(context), m_Backend(backend)
	{
		const auto& path = filepath.string();

		int width, height, channels;
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
		assert(data && "Failed to load data from disk!");

		VkDeviceSize dataSize = width * height * 4;
		m_Width = width;
		m_Height = height;
		m_Format = VK_FORMAT_R8G8B8A8_SRGB;

		CreateImage();
		StoreData(dataSize, data);
		CreateImageView();
		CreateSampler();

		stbi_image_free(data);
	}

	Image::~Image()
	{
		vkDestroySampler(m_Context->GetDevice(), m_Sampler, nullptr);
		vkFreeMemory(m_Context->GetDevice(), m_Memory, nullptr);
		vkDestroyImageView(m_Context->GetDevice(), m_ImageView, nullptr);
		vkDestroyImage(m_Context->GetDevice(), m_Image, nullptr);
	}

	void Image::CreateImage()
	{
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = m_Width;
		imageInfo.extent.height = m_Height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = m_Format;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;

		VK_CHECK(vkCreateImage(m_Context->GetDevice(), &imageInfo, nullptr, &m_Image));
	}

	void Image::CreateImageView()
	{
		VkImageViewCreateInfo imageViewInfo{};
		imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.image = m_Image;
		imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewInfo.format = m_Format;
		imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewInfo.subresourceRange.baseMipLevel = 0;
		imageViewInfo.subresourceRange.levelCount = 1;
		imageViewInfo.subresourceRange.baseArrayLayer = 0;
		imageViewInfo.subresourceRange.layerCount = 1;

		VK_CHECK(vkCreateImageView(m_Context->GetDevice(), &imageViewInfo, nullptr, &m_ImageView));
	}

	void Image::StoreData(size_t dataSize, const void* data)
	{
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		
		BufferUtils::CreateBuffer(dataSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, m_Context->GetDevice(), &stagingBuffer);
		BufferUtils::AllocateMemory(stagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_Context, &stagingBufferMemory);

		ImageUtils::AllocateMemory(m_Image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Context, &m_Memory);

		void* mapped;
		vkMapMemory(m_Context->GetDevice(), stagingBufferMemory, 0, dataSize, 0, &mapped);
		memcpy(mapped, data, dataSize);
		vkUnmapMemory(m_Context->GetDevice(), stagingBufferMemory);

		VkCommandBuffer commandBuffer = m_Backend->AllocateNewCommandBuffer();
		ImageUtils::TransitionLayout(m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, commandBuffer);
		ImageUtils::CopyFromBuffer(stagingBuffer, m_Image, m_Width, m_Height, commandBuffer);
		ImageUtils::TransitionLayout(m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, commandBuffer);
		m_Backend->SubmitCommandBuffer(commandBuffer);

		vkFreeMemory(m_Context->GetDevice(), stagingBufferMemory, nullptr);
		vkDestroyBuffer(m_Context->GetDevice(), stagingBuffer, nullptr);
	}

	void Image::CreateSampler()
	{
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = false;
		samplerInfo.maxAnisotropy = 1.0f;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = false;
		samplerInfo.compareEnable = false;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;

		VK_CHECK(vkCreateSampler(m_Context->GetDevice(), &samplerInfo, nullptr, &m_Sampler));
	}

}
