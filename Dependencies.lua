group "Dependencies"

project "GLFW"
	location "ChessEngine/vendor/GLFW"

	kind "StaticLib"
	language "C"
	staticruntime "off"
	warnings "off"

	targetdir ("/bin/%{prj.name}")
	objdir ("bin-int/%{prj.name}")

	files
	{
		"ChessEngine/vendor/GLFW/include/GLFW/glfw3.h",
		"ChessEngine/vendor/GLFW/include/GLFW/glfw3native.h",
		"ChessEngine/vendor/GLFW/src/glfw_config.h",
		"ChessEngine/vendor/GLFW/src/context.c",
		"ChessEngine/vendor/GLFW/src/init.c",
		"ChessEngine/vendor/GLFW/src/input.c",
		"ChessEngine/vendor/GLFW/src/monitor.c",

		"ChessEngine/vendor/GLFW/src/null_init.c",
		"ChessEngine/vendor/GLFW/src/null_joystick.c",
		"ChessEngine/vendor/GLFW/src/null_monitor.c",
		"ChessEngine/vendor/GLFW/src/null_window.c",

		"ChessEngine/vendor/GLFW/src/platform.c",
		"ChessEngine/vendor/GLFW/src/vulkan.c",
		"ChessEngine/vendor/GLFW/src/window.c",
	}

	filter "system:linux"
		pic "On"

		systemversion "latest"
		
		files
		{
			"ChessEngine/vendor/GLFW/src/x11_init.c",
			"ChessEngine/vendor/GLFW/src/x11_monitor.c",
			"ChessEngine/vendor/GLFW/src/x11_window.c",
			"ChessEngine/vendor/GLFW/src/xkb_unicode.c",
			"ChessEngine/vendor/GLFW/src/posix_module.c",
			"ChessEngine/vendor/GLFW/src/posix_time.c",
			"ChessEngine/vendor/GLFW/src/posix_thread.c",
			"ChessEngine/vendor/GLFW/src/posix_module.c",
			"ChessEngine/vendor/GLFW/src/glx_context.c",
			"ChessEngine/vendor/GLFW/src/egl_context.c",
			"ChessEngine/vendor/GLFW/src/osmesa_context.c",
			"ChessEngine/vendor/GLFW/src/linux_joystick.c"
		}

		defines
		{
			"_GLFW_X11"
		}

	filter "system:macosx"
		pic "On"

		files
		{
			"ChessEngine/vendor/GLFW/src/cocoa_init.m",
			"ChessEngine/vendor/GLFW/src/cocoa_monitor.m",
			"ChessEngine/vendor/GLFW/src/cocoa_window.m",
			"ChessEngine/vendor/GLFW/src/cocoa_joystick.m",
			"ChessEngine/vendor/GLFW/src/cocoa_time.c",
			"ChessEngine/vendor/GLFW/src/nsgl_context.m",
			"ChessEngine/vendor/GLFW/src/posix_thread.c",
			"ChessEngine/vendor/GLFW/src/posix_module.c",
			"ChessEngine/vendor/GLFW/src/osmesa_context.c",
			"ChessEngine/vendor/GLFW/src/egl_context.c"
		}

		defines
		{
			"_GLFW_COCOA"
		}

	filter "system:windows"
		systemversion "latest"

		files
		{
			"ChessEngine/vendor/GLFW/src/win32_init.c",
			"ChessEngine/vendor/GLFW/src/win32_joystick.c",
			"ChessEngine/vendor/GLFW/src/win32_module.c",
			"ChessEngine/vendor/GLFW/src/win32_monitor.c",
			"ChessEngine/vendor/GLFW/src/win32_time.c",
			"ChessEngine/vendor/GLFW/src/win32_thread.c",
			"ChessEngine/vendor/GLFW/src/win32_window.c",
			"ChessEngine/vendor/GLFW/src/wgl_context.c",
			"ChessEngine/vendor/GLFW/src/egl_context.c",
			"ChessEngine/vendor/GLFW/src/osmesa_context.c"
		}

		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "full"

group ""
