#include "pch.h"
#include "ImGuiManager.h"

#include "ChessEngine/Rendering/VulkanUtils.h"

#include <imgui.h>

#include <backends/imgui_impl_glfw.cpp>
#include <backends/imgui_impl_vulkan.cpp>

namespace ChessEngine {

	ImGuiManager::ImGuiManager(GLFWwindow* windowHandle, const std::shared_ptr<RendererContext>& context, const std::shared_ptr<RendererBackend>& backend)
		: m_Context(context), m_Backend(backend)
	{
		IMGUI_CHECKVERSION();

		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		VkDescriptorPoolSize poolSizes[] = {
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 100 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 100 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 100 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 100 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 100 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 100 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 100 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 100 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 100 }
		};
		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		poolInfo.maxSets = 100 * IM_ARRAYSIZE(poolSizes);
		poolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(poolSizes);
		poolInfo.pPoolSizes = poolSizes;
		VK_CHECK(vkCreateDescriptorPool(m_Context->GetDevice(), &poolInfo, nullptr, &m_DescriptorPool));

		ImGui_ImplGlfw_InitForVulkan(windowHandle, true);

		ImGui_ImplVulkan_InitInfo initInfo{};
		initInfo.Instance = m_Context->GetInstance();
		initInfo.PhysicalDevice = m_Context->GetPhysicalDevice();
		initInfo.Device = m_Context->GetDevice();
		initInfo.QueueFamily = m_Context->GetGraphicsQueueFamilyIndex();
		initInfo.Queue = m_Context->GetGraphicsQueue();
		initInfo.DescriptorPool = m_DescriptorPool;
		initInfo.RenderPass = m_Backend->GetSwapchainRenderPass();
		initInfo.MinImageCount = 2;
		initInfo.ImageCount = m_Backend->GetImageCount();
		initInfo.CheckVkResultFn = [](VkResult result) { VK_CHECK(result); };

		ImGui_ImplVulkan_Init(&initInfo);
	}

	ImGuiManager::~ImGuiManager()
	{
		m_Context->WaitIdle();
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		vkResetDescriptorPool(m_Context->GetDevice(), m_DescriptorPool, 0);
		vkDestroyDescriptorPool(m_Context->GetDevice(), m_DescriptorPool, nullptr);
	}

	void ImGuiManager::BeginFrame()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiManager::EndFrame()
	{
		ImGui::Render();

		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_Backend->GetCurrentCommandBuffer());

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

}
