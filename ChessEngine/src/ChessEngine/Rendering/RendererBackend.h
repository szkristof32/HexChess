#pragma once

#include "ChessEngine/Rendering/RendererContext.h"

#include <vulkan/vulkan.h>

namespace ChessEngine {

	class Pipeline;

	class RendererBackend
	{
	public:
		RendererBackend(const std::shared_ptr<RendererContext>& context, GLFWwindow* windowHandle);
		~RendererBackend();

		void BeginFrame();
		void EndFrame();

		void BindPipeline(std::weak_ptr<Pipeline> pipeline) const;
		void Draw(uint32_t vertexCount) const;

		VkRenderPass GetSwapchainRenderPass() const { return m_SwapchainRenderPass; }
	private:
		void Reload();
		void CreateSwapchain();
		void GetImageResources();
		void CreateSwapchainRenderPass();
		void CreateFramebuffers();
		void CreateCommandBuffers();
		void CreateSyncObjects();

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

		VkSemaphore m_ImageAvailable;
		VkSemaphore m_RenderFinished;
		VkFence m_FrameInFlight;
	};

}
