#include "pch.h"
#include "RendererBackend.h"

#include "ChessEngine/Rendering/Pipeline.h"
#include "ChessEngine/Rendering/Buffers.h"

#include "VulkanUtils.h"

#include <GLFW/glfw3.h>

namespace ChessEngine {

	RendererBackend::RendererBackend(const std::shared_ptr<RendererContext>& context, GLFWwindow* windowHandle)
		: m_Context(context), m_WindowHandle(windowHandle)
	{
		Reload();
	}

	RendererBackend::~RendererBackend()
	{
		vkDeviceWaitIdle(m_Context->GetDevice());

		for (uint32_t i = 0;i < RendererBackend::MaxFramesInFlight;i++)
		{
			vkDestroyFence(m_Context->GetDevice(), m_FrameInFlight[i], nullptr);
			vkDestroySemaphore(m_Context->GetDevice(), m_RenderFinished[i], nullptr);
			vkDestroySemaphore(m_Context->GetDevice(), m_ImageAvailable[i], nullptr);
		}

		for (uint32_t i = 0;i < m_SwapchainImageCount;i++)
		{
			vkDestroyImageView(m_Context->GetDevice(), m_SwapchainImageViews[i], nullptr);
			vkDestroyFramebuffer(m_Context->GetDevice(), m_SwapchainFramebuffers[i], nullptr);
		}

		vkDestroyCommandPool(m_Context->GetDevice(), m_CommandPool, nullptr);

		vkDestroyRenderPass(m_Context->GetDevice(), m_SwapchainRenderPass, nullptr);
		vkDestroySwapchainKHR(m_Context->GetDevice(), m_Swapchain, nullptr);
	}

	bool RendererBackend::BeginFrame()
	{
		vkWaitForFences(m_Context->GetDevice(), 1, &m_FrameInFlight[m_FrameIndex], true, std::numeric_limits<uint64_t>::max());

		VkResult result = vkAcquireNextImageKHR(m_Context->GetDevice(), m_Swapchain, std::numeric_limits<uint64_t>::max(), m_ImageAvailable[m_FrameIndex], nullptr, &m_ImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			Reload();
			return false;
		}
		if (result != VK_SUBOPTIMAL_KHR)
		{
			VK_CHECK(result);
		}

		vkResetFences(m_Context->GetDevice(), 1, &m_FrameInFlight[m_FrameIndex]);

		VkCommandBufferBeginInfo commandBufferBegin{};
		commandBufferBegin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		VK_CHECK(vkResetCommandBuffer(m_CommandBuffers[m_FrameIndex], 0));
		VK_CHECK(vkBeginCommandBuffer(m_CommandBuffers[m_FrameIndex], &commandBufferBegin));

		VkClearValue clearColour{};
		clearColour.color = { 1.0f, 0.0f, 1.0f, 1.0f };

		VkRenderPassBeginInfo renderPassBegin{};
		renderPassBegin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBegin.renderPass = m_SwapchainRenderPass;
		renderPassBegin.framebuffer = m_SwapchainFramebuffers[m_ImageIndex];
		renderPassBegin.renderArea.offset = { 0, 0 };
		renderPassBegin.renderArea.extent = m_SwapchainExtent;
		renderPassBegin.clearValueCount = 1;
		renderPassBegin.pClearValues = &clearColour;

		vkCmdBeginRenderPass(m_CommandBuffers[m_FrameIndex], &renderPassBegin, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = (float)m_SwapchainExtent.height;
		viewport.width = (float)m_SwapchainExtent.width;
		viewport.height = -(float)m_SwapchainExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		vkCmdSetViewport(m_CommandBuffers[m_FrameIndex], 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = m_SwapchainExtent;

		vkCmdSetScissor(m_CommandBuffers[m_FrameIndex], 0, 1, &scissor);

		return true;
	}

	bool RendererBackend::EndFrame()
	{
		vkCmdEndRenderPass(m_CommandBuffers[m_FrameIndex]);

		VK_CHECK(vkEndCommandBuffer(m_CommandBuffers[m_FrameIndex]));

		VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &m_ImageAvailable[m_FrameIndex];
		submitInfo.pWaitDstStageMask = &waitStage;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffers[m_FrameIndex];
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &m_RenderFinished[m_FrameIndex];

		VK_CHECK(vkQueueSubmit(m_Context->GetGraphicsQueue(), 1, &submitInfo, m_FrameInFlight[m_FrameIndex]));

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &m_RenderFinished[m_FrameIndex];
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_Swapchain;
		presentInfo.pImageIndices = &m_ImageIndex;

		VkResult result = vkQueuePresentKHR(m_Context->GetGraphicsQueue(), &presentInfo);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_FramebufferResized)
		{
			m_FramebufferResized = false;
			Reload();
		}
		else
		{
			VK_CHECK(result);
		}

		m_FrameIndex = (m_FrameIndex + 1) % RendererBackend::MaxFramesInFlight;

		return true;
	}

	void RendererBackend::BindPipeline(std::weak_ptr<Pipeline> pipeline) const
	{
		vkCmdBindPipeline(m_CommandBuffers[m_FrameIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.lock()->GetPipeline());
	}

	void RendererBackend::BindVertexBuffer(std::weak_ptr<VertexBuffer> vertexBuffer) const
	{
		VkBuffer buffer = vertexBuffer.lock()->GetBuffer();
		VkDeviceSize offset = 0;

		vkCmdBindVertexBuffers(m_CommandBuffers[m_FrameIndex], 0, 1, &buffer, &offset);
	}

	void RendererBackend::BindIndexBuffer(std::weak_ptr<IndexBuffer> indexBuffer) const
	{
		vkCmdBindIndexBuffer(m_CommandBuffers[m_FrameIndex], indexBuffer.lock()->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
	}

	void RendererBackend::Draw(uint32_t vertexCount) const
	{
		vkCmdDraw(m_CommandBuffers[m_FrameIndex], vertexCount, 1, 0, 0);
	}

	void RendererBackend::DrawIndexed(uint32_t indexCount) const
	{
		vkCmdDrawIndexed(m_CommandBuffers[m_FrameIndex], indexCount, 1, 0, 0, 0);
	}

	VkCommandBuffer RendererBackend::AllocateNewCommandBuffer()
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_CommandPool;
		allocInfo.commandBufferCount = 1;
		
		VkCommandBuffer commandBuffer;
		VK_CHECK(vkAllocateCommandBuffers(m_Context->GetDevice(), &allocInfo, &commandBuffer));

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		VK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo));

		return commandBuffer;
	}

	void RendererBackend::SubmitCommandBuffer(VkCommandBuffer commandBuffer)
	{
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		VK_CHECK(vkEndCommandBuffer(commandBuffer));

		VK_CHECK(vkQueueSubmit(m_Context->GetGraphicsQueue(), 1, &submitInfo, nullptr));
		VK_CHECK(vkQueueWaitIdle(m_Context->GetGraphicsQueue()));

		vkFreeCommandBuffers(m_Context->GetDevice(), m_CommandPool, 1, &commandBuffer);
	}

	void RendererBackend::OnResize(uint32_t width, uint32_t height)
	{
		m_FramebufferResized = true;
	}

	void RendererBackend::Reload()
	{
		VkSwapchainKHR oldSwapchain = m_Swapchain;

		int width = 0, height = 0;
		glfwGetFramebufferSize(m_WindowHandle, &width, &height);
		while (width == 0 || height == 0)
		{
			glfwGetFramebufferSize(m_WindowHandle, &width, &height);
			glfwWaitEvents();
		}

		if (!oldSwapchain)
		{
			SwapchainFormatInfo swapchainFormat = m_Context->GetSwapchainFormatInfo();

			m_SurfaceFormat = ChooseSurfaceFormat(swapchainFormat.Formats);
			m_PresentMode = ChoosePresentMode(swapchainFormat.PresentModes);
			m_SwapchainExtent = ChooseSwapchainExtent(swapchainFormat.Capabilities);

			CreateSwapchainRenderPass();
			CreateCommandBuffers();
			CreateSyncObjects();
		}

		m_Context->WaitIdle();

		for (size_t i = 0; i < m_SwapchainImageCount; i++) {
			vkDestroyFramebuffer(m_Context->GetDevice(), m_SwapchainFramebuffers[i], nullptr);
			vkDestroyImageView(m_Context->GetDevice(), m_SwapchainImageViews[i], nullptr);
		}

		CreateSwapchain();
		GetImageResources();
		CreateFramebuffers();

		vkDestroySwapchainKHR(m_Context->GetDevice(), oldSwapchain, nullptr);
	}

	void RendererBackend::CreateSwapchain()
	{
		SwapchainFormatInfo swapchainFormat = m_Context->GetSwapchainFormatInfo();

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
		swapchainInfo.imageFormat = m_SurfaceFormat.format;
		swapchainInfo.imageColorSpace = m_SurfaceFormat.colorSpace;
		swapchainInfo.imageExtent = m_SwapchainExtent;
		swapchainInfo.imageArrayLayers = 1;
		swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainInfo.imageSharingMode = queueFamilyIndices.size() > 1 ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE;
		swapchainInfo.queueFamilyIndexCount = (uint32_t)indices.size();
		swapchainInfo.pQueueFamilyIndices = indices.data();
		swapchainInfo.preTransform = swapchainFormat.Capabilities.currentTransform;
		swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainInfo.presentMode = m_PresentMode;
		swapchainInfo.clipped = true;
		swapchainInfo.oldSwapchain = m_Swapchain;

		VK_CHECK(vkCreateSwapchainKHR(m_Context->GetDevice(), &swapchainInfo, nullptr, &m_Swapchain));
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
			imageViewInfo.format = m_SurfaceFormat.format;
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
		colourAttachment.format = m_SurfaceFormat.format;
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

	void RendererBackend::CreateFramebuffers()
	{
		m_SwapchainFramebuffers.resize(m_SwapchainImageCount);

		for (uint32_t i = 0;i < m_SwapchainImageCount;i++)
		{
			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = m_SwapchainRenderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = &m_SwapchainImageViews[i];
			framebufferInfo.width = m_SwapchainExtent.width;
			framebufferInfo.height = m_SwapchainExtent.height;
			framebufferInfo.layers = 1;

			VK_CHECK(vkCreateFramebuffer(m_Context->GetDevice(), &framebufferInfo, nullptr, &m_SwapchainFramebuffers[i]));
		}
	}

	void RendererBackend::CreateCommandBuffers()
	{
		VkCommandPoolCreateInfo commandPoolInfo{};
		commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		commandPoolInfo.queueFamilyIndex = m_Context->GetGraphicsQueueFamilyIndex();

		VK_CHECK(vkCreateCommandPool(m_Context->GetDevice(), &commandPoolInfo, nullptr, &m_CommandPool));

		for (uint32_t i = 0;i < RendererBackend::MaxFramesInFlight;i++)
		{
			VkCommandBufferAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandPool = m_CommandPool;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandBufferCount = 1;

			VK_CHECK(vkAllocateCommandBuffers(m_Context->GetDevice(), &allocInfo, &m_CommandBuffers[i]));
		}
	}

	void RendererBackend::CreateSyncObjects()
	{
		for (uint32_t i = 0;i < RendererBackend::MaxFramesInFlight;i++)
		{
			VkSemaphoreCreateInfo semaphoreInfo{};
			semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			VkFenceCreateInfo fenceInfo{};
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

			VK_CHECK(vkCreateSemaphore(m_Context->GetDevice(), &semaphoreInfo, nullptr, &m_ImageAvailable[i]));
			VK_CHECK(vkCreateSemaphore(m_Context->GetDevice(), &semaphoreInfo, nullptr, &m_RenderFinished[i]));
			VK_CHECK(vkCreateFence(m_Context->GetDevice(), &fenceInfo, nullptr, &m_FrameInFlight[i]));
		}
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
