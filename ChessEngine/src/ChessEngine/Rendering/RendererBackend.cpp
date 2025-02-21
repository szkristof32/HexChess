#include "pch.h"
#include "RendererBackend.h"

#include "VulkanUtils.h"

#include <GLFW/glfw3.h>

namespace ChessEngine {

	RendererBackend::RendererBackend(const std::shared_ptr<RendererContext>& context, GLFWwindow* windowHandle)
		: m_Context(context), m_WindowHandle(windowHandle)
	{
		RecreateSwapchain();
	}

	RendererBackend::~RendererBackend()
	{
		for (const auto& imageView : m_SwapchainImageViews)
		{
			vkDestroyImageView(m_Context->GetDevice(), imageView, nullptr);
		}

		vkDestroyRenderPass(m_Context->GetDevice(), m_SwapchainRenderPass, nullptr);
		vkDestroySwapchainKHR(m_Context->GetDevice(), m_Swapchain, nullptr);
	}

	void RendererBackend::BeginFrame()
	{
	}

	void RendererBackend::EndFrame()
	{
	}

	void RendererBackend::RecreateSwapchain()
	{
		SwapchainFormatInfo swapchainFormat = m_Context->GetSwapchainFormatInfo();

		VkSurfaceFormatKHR surfaceFormat = ChooseSurfaceFormat(swapchainFormat.Formats);
		VkPresentModeKHR presentMode = ChoosePresentMode(swapchainFormat.PresentModes);
		VkExtent2D extent = ChooseSwapchainExtent(swapchainFormat.Capabilities);

		m_SwapchainFormat = surfaceFormat.format;

		uint32_t imageCount = swapchainFormat.Capabilities.minImageCount + 1;
		if (swapchainFormat.Capabilities.maxImageCount > 0 &&
			imageCount > swapchainFormat.Capabilities.maxImageCount)
		{
			imageCount = swapchainFormat.Capabilities.maxImageCount;
		}
		
		std::set<uint32_t> queueFamilyIndices = { m_Context->GetGraphicsQueueFamilyIndex() };
		std::vector<uint32_t> indices(queueFamilyIndices.begin(), queueFamilyIndices.end());

		VkSwapchainCreateInfoKHR swapchainInfo{};
		swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainInfo.surface = m_Context->GetSurface();
		swapchainInfo.minImageCount = imageCount;
		swapchainInfo.imageFormat = surfaceFormat.format;
		swapchainInfo.imageColorSpace = surfaceFormat.colorSpace;
		swapchainInfo.imageExtent = extent;
		swapchainInfo.imageArrayLayers = 1;
		swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainInfo.imageSharingMode = queueFamilyIndices.size() > 1 ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE;
		swapchainInfo.queueFamilyIndexCount = (uint32_t)indices.size();
		swapchainInfo.pQueueFamilyIndices = indices.data();
		swapchainInfo.preTransform = swapchainFormat.Capabilities.currentTransform;
		swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainInfo.presentMode = presentMode;
		swapchainInfo.clipped = true;
		swapchainInfo.oldSwapchain = nullptr;

		VK_CHECK(vkCreateSwapchainKHR(m_Context->GetDevice(), &swapchainInfo, nullptr, &m_Swapchain));

		GetImageResources();
		CreateSwapchainRenderPass();
	}

	void RendererBackend::GetImageResources()
	{
		vkGetSwapchainImagesKHR(m_Context->GetDevice(), m_Swapchain, &m_SwapchainImageCount, nullptr);
		m_SwapchainImages.resize(m_SwapchainImageCount);
		vkGetSwapchainImagesKHR(m_Context->GetDevice(), m_Swapchain, &m_SwapchainImageCount, m_SwapchainImages.data());

		m_SwapchainImageViews.resize(m_SwapchainImageCount);
		for (uint32_t i = 0;i < m_SwapchainImageCount;i++)
		{
			VkImageViewCreateInfo imageViewInfo{};
			imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewInfo.image = m_SwapchainImages[i];
			imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewInfo.format = m_SwapchainFormat;
			imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageViewInfo.subresourceRange.baseMipLevel = 0;
			imageViewInfo.subresourceRange.levelCount = 1;
			imageViewInfo.subresourceRange.baseArrayLayer = 0;
			imageViewInfo.subresourceRange.layerCount = 1;

			VK_CHECK(vkCreateImageView(m_Context->GetDevice(), &imageViewInfo, nullptr, &m_SwapchainImageViews[i]));
		}
	}

	void RendererBackend::CreateSwapchainRenderPass()
	{
		VkAttachmentDescription colourAttachment{};
		colourAttachment.format = m_SwapchainFormat;
		colourAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colourAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colourAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colourAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colourAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colourAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colourAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colourAttachmentReference{};
		colourAttachmentReference.attachment = 0;
		colourAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colourAttachmentReference;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colourAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;

		VK_CHECK(vkCreateRenderPass(m_Context->GetDevice(), &renderPassInfo, nullptr, &m_SwapchainRenderPass));
	}

	VkSurfaceFormatKHR RendererBackend::ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats)
	{
		for (const auto& availableFormat : formats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
				availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}

		return formats[0];
	}

	VkPresentModeKHR RendererBackend::ChoosePresentMode(const std::vector<VkPresentModeKHR>& presentModes)
	{
		for (const auto& availablePresentMode : presentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return availablePresentMode;
			}

			if (availablePresentMode == VK_PRESENT_MODE_FIFO_RELAXED_KHR)
			{
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D RendererBackend::ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		}
		else {
			int width, height;
			glfwGetFramebufferSize(m_WindowHandle, &width, &height);

			VkExtent2D actualExtent = {
				(uint32_t)width,
				(uint32_t)height
			};

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

}
