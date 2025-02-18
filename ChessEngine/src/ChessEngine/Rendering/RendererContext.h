#pragma once

#include <vulkan/vulkan.h>

struct GLFWwindow;

namespace ChessEngine {

	struct QueueFamilyIndices
	{
		std::optional<uint32_t> GraphicsFamily;

		bool IsComplete() { return GraphicsFamily.has_value(); }
	};

	class RendererContext
	{
	public:
		RendererContext(GLFWwindow* windowHandle);
		~RendererContext();

		void BeginFrame();
		void EndFrame();
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

#ifdef DEBUG
		VkDebugUtilsMessengerEXT m_DebugMessenger;
#endif
	};

}
