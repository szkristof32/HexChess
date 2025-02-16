#pragma once

#include "ChessEngine/Core/Core.h"

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

#ifdef DEBUG
	#define VK_CHECK(expr) { VkResult result = expr; if (result != VK_SUCCESS) { std::cerr << COLOR_RED "Failed: " << string_VkResult(result) << "\n  " #expr << "\n" COLOR_RESET; DEBUGBREAK(); } }
#else
	#define VK_CHECK(expr) expr
#endif
