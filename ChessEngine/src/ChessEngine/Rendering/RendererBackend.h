#pragma once

#include <vulkan/vulkan.h>

namespace ChessEngine {

	struct QueueFamilyIndices
	{
		std::optional<uint32_t> GraphicsFamily;

		bool IsComplete() { return GraphicsFamily.has_value(); }
	};

	class RendererBackend
	{
	public:
		RendererBackend();
		~RendererBackend();

		void BeginFrame();
		void EndFrame();
	private:
		void CreateInstance();
		void PickPhysicalDevice();
		void CreateDevice();

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

		VkQueue m_GraphicsQueue;
		uint32_t m_GraphicsQueueFamily;

#ifdef DEBUG
		VkDebugUtilsMessengerEXT m_DebugMessenger;
#endif
	};

}
