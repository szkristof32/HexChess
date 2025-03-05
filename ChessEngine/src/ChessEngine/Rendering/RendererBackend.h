#pragma once

#include "ChessEngine/Rendering/RendererContext.h"

#include <vulkan/vulkan.h>

namespace ChessEngine {

	class Pipeline;
	class VertexBuffer;
	class IndexBuffer;

	class RendererBackend
	{
	public:
		constexpr static uint32_t MaxFramesInFlight = 2;
	public:
		RendererBackend(const std::shared_ptr<RendererContext>& context, GLFWwindow* windowHandle);
		~RendererBackend();

		bool BeginFrame();
		bool EndFrame();

		void BindPipeline(std::weak_ptr<Pipeline> pipeline) const;
		void BindVertexBuffer(std::weak_ptr<VertexBuffer> vertexBuffer) const;
		void BindIndexBuffer(std::weak_ptr<IndexBuffer> indexBuffer) const;

		void Draw(uint32_t vertexCount) const;
		void DrawIndexed(uint32_t indexCount) const;

		VkRenderPass GetSwapchainRenderPass() const { return m_SwapchainRenderPass; }
		VkCommandBuffer GetCurrentCommandBuffer() const { return m_CommandBuffers[m_FrameIndex]; }
		VkCommandBuffer AllocateNewCommandBuffer();
		void SubmitCommandBuffer(VkCommandBuffer commandBuffer);

		VkSampleCountFlagBits GetMaxMSAASamples() const { return m_MSAASamples; }
		uint32_t GetImageCount() const { return m_SwapchainImageCount; }
	public:
		void OnResize(uint32_t width, uint32_t height);
	private:
		void Reload();
		void CreateSwapchain();
		void GetImageResources();
		void CreateDepthResources();
		void CreateSwapchainRenderPass();
		void CreateFramebuffers();
		void CreateCommandBuffers();
		void CreateSyncObjects();

		void SetResizedFlag() { m_FramebufferResized = true; }

		VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
		VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& presentModes);
		VkExtent2D ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		VkSampleCountFlagBits GetMSAASamples();
	private:
		std::shared_ptr<RendererContext> m_Context;
		GLFWwindow* m_WindowHandle;

		VkSwapchainKHR m_Swapchain = nullptr;
		bool m_FramebufferResized = false;

		VkSurfaceFormatKHR m_SurfaceFormat;
		VkPresentModeKHR m_PresentMode;
		VkExtent2D m_SwapchainExtent;

		uint32_t m_SwapchainImageCount = 0;
		uint32_t m_ImageIndex = 0;
		uint32_t m_FrameIndex = 0;

		std::vector<VkImage> m_SwapchainImages;
		std::vector<VkImageView> m_SwapchainImageViews;

		VkImage m_RenderTarget = nullptr;
		VkImageView m_RenderTargetView = nullptr;
		VkDeviceMemory m_RenderTargetMemory = nullptr;

		VkImage m_DepthImage = nullptr;
		VkImageView m_DepthImageView = nullptr;
		VkDeviceMemory m_DepthMemory = nullptr;
		VkFormat m_DepthFormat;

		VkSampleCountFlagBits m_MSAASamples = VK_SAMPLE_COUNT_1_BIT;

		std::vector<VkFramebuffer> m_SwapchainFramebuffers;

		VkRenderPass m_SwapchainRenderPass;

		VkCommandPool m_CommandPool;
		std::array<VkCommandBuffer, MaxFramesInFlight> m_CommandBuffers;

		std::array<VkSemaphore, MaxFramesInFlight> m_ImageAvailable;
		std::array<VkSemaphore, MaxFramesInFlight> m_RenderFinished;
		std::array<VkFence, MaxFramesInFlight> m_FrameInFlight;
	};

}
