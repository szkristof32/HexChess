#pragma once

#include "ChessEngine/Rendering/RendererContext.h"
#include "ChessEngine/Rendering/RendererBackend.h"

struct GLFWwindow;

namespace ChessEngine {

	class ImGuiManager
	{
	public:
		ImGuiManager(GLFWwindow* windowHandle, const std::shared_ptr<RendererContext>& context, const std::shared_ptr<RendererBackend>& backend);
		~ImGuiManager();

		void BeginFrame();
		void EndFrame();
	private:
		std::shared_ptr<RendererContext> m_Context;
		std::shared_ptr<RendererBackend> m_Backend;

		VkDescriptorPool m_DescriptorPool;
	};

}
