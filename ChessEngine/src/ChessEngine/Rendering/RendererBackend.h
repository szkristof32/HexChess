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
	private:
		void RecreateSwapchain();

		VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
		VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& presentModes);
		VkExtent2D ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	private:
		std::shared_ptr<RendererContext> m_Context;
		GLFWwindow* m_WindowHandle;

		VkSwapchainKHR m_Swapchain;
	};

}
