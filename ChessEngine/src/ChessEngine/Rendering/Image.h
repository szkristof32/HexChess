#pragma once

#include "ChessEngine/Rendering/RendererContext.h"
#include "ChessEngine/Rendering/RendererBackend.h"

namespace ChessEngine {

	namespace ImageUtils {

		void AllocateMemory(VkImage image, VkMemoryPropertyFlags memoryProperties, std::weak_ptr<::ChessEngine::RendererContext> context, VkDeviceMemory* outMemory);
		void CopyFromBuffer(VkBuffer source, VkImage destination, uint32_t width, uint32_t height, VkCommandBuffer commandBuffer);
		void TransitionLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkCommandBuffer commandBuffer, uint32_t mipLevels = 1);

	}

	class Image
	{
	public:
		Image(const std::filesystem::path& filepath, const std::shared_ptr<RendererContext>& context, const std::shared_ptr<RendererBackend>& backend);
		~Image();

		VkImage GetImage() const { return m_Image; }
		VkImageView GetImageView() const { return m_ImageView; }
		VkSampler GetSampler() const { return m_Sampler; }
	private:
		void CreateImage();
		void CreateImageView();
		void StoreData(size_t dataSize, const void* data);
		void GenerateMipmaps();
		void CreateSampler();
	private:
		std::shared_ptr<RendererContext> m_Context;
		std::shared_ptr<RendererBackend> m_Backend;

		VkImage m_Image;
		VkImageView m_ImageView;
		VkDeviceMemory m_Memory;

		uint32_t m_Width, m_Height;
		uint32_t m_MipLevels;
		VkFormat m_Format;
		VkSampler m_Sampler;
	};

}
