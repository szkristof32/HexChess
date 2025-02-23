#pragma once

#include <vulkan/vulkan.h>

struct GLFWwindow;

namespace ChessEngine {

	struct QueueFamilyIndices
	{
		std::optional<uint32_t> GraphicsFamily;

		bool IsComplete() const { return GraphicsFamily.has_value(); }
	};

	struct SwapchainFormatInfo
	{
		VkSurfaceCapabilitiesKHR Capabilities;
		std::vector<VkSurfaceFormatKHR> Formats;
		std::vector<VkPresentModeKHR> PresentModes;

		bool IsAdequate() const { return !Formats.empty() && !PresentModes.empty(); }
	};

	class RendererContext
	{
	public:
		RendererContext(GLFWwindow* windowHandle);
		~RendererContext();

		void BeginFrame();
		void EndFrame();

		void WaitIdle() const;

		VkInstance GetInstance() const { return m_Instance; }
		VkPhysicalDevice GetPhysicalDevice() const { return m_PhysicalDevice; }
		VkDevice GetDevice() const { return m_Device; }
		VkSurfaceKHR GetSurface() const { return m_Surface; }

		VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }
		uint32_t GetGraphicsQueueFamilyIndex() const { return m_GraphicsQueueFamily; }
		const SwapchainFormatInfo& GetSwapchainFormatInfo() const { return m_SwapchainFormat; }

		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	private:
		void CreateInstance();
		void PickPhysicalDevice();
		void CreateDevice();
		void CreateSurface(GLFWwindow* windowHandle);

		std::vector<const char*> GetInstanceExtensions();
		std::vector<const char*> GetDeviceExtensions();
		std::vector<const char*> GetValidationLayers();

		bool IsDeviceSuitable(VkPhysicalDevice device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		bool CheckSupportedExtensions(VkPhysicalDevice device);
		SwapchainFormatInfo QuerySwapchainFormatInfo(VkPhysicalDevice device);

#ifdef DEBUG
		void CreateDebugMessenger();
		void DestroyDebugMessenger();
		bool CheckValidationLayer();
		VkDebugUtilsMessengerCreateInfoEXT DebugMessengerInfo();
#endif
	private:
		VkInstance m_Instance;
		VkPhysicalDevice m_PhysicalDevice;
		VkDevice m_Device;
		VkSurfaceKHR m_Surface;

		VkQueue m_GraphicsQueue;
		uint32_t m_GraphicsQueueFamily;
		SwapchainFormatInfo m_SwapchainFormat;

#ifdef DEBUG
		VkDebugUtilsMessengerEXT m_DebugMessenger;
#endif
	};

}
