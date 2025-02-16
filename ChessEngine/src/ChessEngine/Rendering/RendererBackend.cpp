#include "pch.h"
#include "RendererBackend.h"

#include "VulkanUtils.h"

#include <GLFW/glfw3.h>

namespace ChessEngine {

#ifdef DEBUG
	#define CreateDebugMessenger() CreateDebugMessenger()
	#define DestroyDebugMessenger() DestroyDebugMessenger()
#else
	#define CreateDebugMessenger()
	#define DestroyDebugMessenger()
#endif

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* data, void*)
	{
		const char* color = COLOR_RESET;
		switch (severity)
		{
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:		color = COLOR_GREEN; break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:	color = COLOR_YELLOW; break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:		color = COLOR_RED; break;
		}

		std::cerr << color << "[Vulkan] " << data->pMessage << COLOR_RESET "\n";

		return false;
	}

	RendererBackend::RendererBackend()
	{
		CreateInstance();
		CreateDebugMessenger();
		PickPhysicalDevice();
		CreateDevice();
	}

	RendererBackend::~RendererBackend()
	{
		vkDestroyDevice(m_Device, nullptr);
		DestroyDebugMessenger();
		vkDestroyInstance(m_Instance, nullptr);
	}

	void RendererBackend::BeginFrame()
	{
	}

	void RendererBackend::EndFrame()
	{
	}

	void RendererBackend::CreateInstance()
	{
#ifdef DEBUG
		if (!CheckValidationLayer())
			return;
#endif

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "HexChess";
		appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
		appInfo.pEngineName = "ChessEngine";
		appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_3;

		auto instanceExtensions = GetInstanceExtensions();
		auto validationLayers = GetValidationLayers();

#ifdef DEBUG
		auto messengerInfo = DebugMessengerInfo();
#endif

		VkInstanceCreateInfo instanceInfo{};
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceInfo.pApplicationInfo = &appInfo;
		instanceInfo.enabledExtensionCount = (uint32_t)instanceExtensions.size();
		instanceInfo.ppEnabledExtensionNames = instanceExtensions.data();
		instanceInfo.enabledLayerCount = (uint32_t)validationLayers.size();
		instanceInfo.ppEnabledLayerNames = validationLayers.data();
#ifdef DEBUG
		instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&messengerInfo;
#endif

		VK_CHECK(vkCreateInstance(&instanceInfo, nullptr, &m_Instance));
	}

	void RendererBackend::PickPhysicalDevice()
	{
		uint32_t deviceCount;
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());

		for (const auto& device : devices)
		{
			if (IsDeviceSuitable(device))
			{
				m_PhysicalDevice = device;
				break;
			}
		}
	}

	void RendererBackend::CreateDevice()
	{
		QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);
		float queuePriority = 1.0f;

		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = *indices.GraphicsFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		auto extensions = GetDeviceExtensions();

		VkDeviceCreateInfo deviceInfo{};
		deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceInfo.queueCreateInfoCount = 1;
		deviceInfo.pQueueCreateInfos = &queueCreateInfo;
		deviceInfo.enabledExtensionCount = (uint32_t)extensions.size();
		deviceInfo.ppEnabledExtensionNames = extensions.data();

		VK_CHECK(vkCreateDevice(m_PhysicalDevice, &deviceInfo, nullptr, &m_Device));

		vkGetDeviceQueue(m_Device, *indices.GraphicsFamily, 0, &m_GraphicsQueue);
		m_GraphicsQueueFamily = *indices.GraphicsFamily;
	}

	std::vector<const char*> RendererBackend::GetInstanceExtensions()
	{
		std::vector<const char*> extensions = {
#ifdef DEBUG
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif
		};

		uint32_t glfwExtensionCount;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		extensions.reserve(extensions.size() + glfwExtensionCount);

		for (uint32_t i = 0;i < glfwExtensionCount;i++)
			extensions.emplace_back(glfwExtensions[i]);

		return extensions;
	}

	std::vector<const char*> RendererBackend::GetDeviceExtensions()
	{
		std::vector<const char*> extensions = {
		};
		return extensions;
	}

	std::vector<const char*> RendererBackend::GetValidationLayers()
	{
		std::vector<const char*> validationLayers = {
#ifdef DEBUG
			"VK_LAYER_KHRONOS_validation"
#endif
		};
		return validationLayers;
	}

	bool RendererBackend::IsDeviceSuitable(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices = FindQueueFamilies(device);

		return indices.IsComplete();
	}

	QueueFamilyIndices RendererBackend::FindQueueFamilies(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices{};

		uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.GraphicsFamily = i;
			}

			i++;
		}

		return indices;
	}

#ifdef DEBUG

	void RendererBackend::CreateDebugMessenger()
	{
		auto messengerInfo = DebugMessengerInfo();

		auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkCreateDebugUtilsMessengerEXT");
		if (vkCreateDebugUtilsMessengerEXT)
			vkCreateDebugUtilsMessengerEXT(m_Instance, &messengerInfo, nullptr, &m_DebugMessenger);
	}

	void RendererBackend::DestroyDebugMessenger()
	{
		auto vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkDestroyDebugUtilsMessengerEXT");
		if (vkDestroyDebugUtilsMessengerEXT)
			vkDestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
	}

	bool RendererBackend::CheckValidationLayer()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> layers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

		auto validationLayers = GetValidationLayers();

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : layers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}

	VkDebugUtilsMessengerCreateInfoEXT RendererBackend::DebugMessengerInfo()
	{
		VkDebugUtilsMessengerCreateInfoEXT messengerInfo{};
		messengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		messengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		messengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
		messengerInfo.pfnUserCallback = DebugCallback;
		messengerInfo.pUserData = nullptr;

		return messengerInfo;
	}

#endif

}
