#pragma once

#include "ChessEngine/Rendering/RendererContext.h"

#include <vulkan/vulkan.h>

namespace ChessEngine {

	class RendererBackend
	{
	public:
		RendererBackend(const std::shared_ptr<RendererContext>& context, GLFWwindow* windowHandle);
		~RendererBackend();

		void BeginFrame();
		void EndFrame();

		VkRenderPass GetSwapchainRenderPass() const { return m_SwapchainRenderPass; }
	private:
		void RecreateSwapchain();
		void GetImageResources();
		void CreateSwapchainRenderPass();
		void CreateFramebuffers();
		void CreateCommandBuffers();

		VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
		VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& presentModes);
		VkExtent2D ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	private:
		std::shared_ptr<RendererContext> m_Context;
		GLFWwindow* m_WindowHandle;

		VkSwapchainKHR m_Swapchain;
		VkFormat m_SwapchainFormat;
		VkExtent2D m_SwapchainExtent;

		uint32_t m_SwapchainImageCount = 0;
		uint32_t m_ImageIndex = 0;

		std::vector<VkImage> m_SwapchainImages;
		std::vector<VkImageView> m_SwapchainImageViews;

		std::vector<VkFramebuffer> m_SwapchainFramebuffers;

		VkRenderPass m_SwapchainRenderPass;

		VkCommandPool m_CommandPool;
		VkCommandBuffer m_CommandBuffer;
	};

}
