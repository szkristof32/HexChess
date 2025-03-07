include "premake/solution_items.lua"

VULKAN_SDK = os.getenv("VULKAN_SDK")

workspace "HexChess"
	startproject "HexChess"

	architecture "x86_64"
	language "C++"
	cppdialect "C++23"

	flags
	{
		"MultiProcessorCompile"
	}

	solution_items
	{
		".editorconfig",
		"premake5.lua",
		"Dependencies.lua"
	}

	configurations
	{
		"Debug",
		"Release"
	}

	filter "action:vs*"
		linkoptions { "/ignore:4099" }
        disablewarnings { "4068" }

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
		optimize "off"
		defines "DEBUG"

	filter "configurations:Release"
		runtime "Release"
		symbols "off"
		optimize "full"
		defines "RELEASE"

include "Dependencies.lua"

project "ChessEngine"
	location "%{prj.name}"
	kind "StaticLib"

	targetdir "bin/%{cfg.buildcfg}/%{prj.name}"
	objdir "bin-int/%{cfg.buildcfg}/%{prj.name}"

	pchheader "pch.h"
	pchsource "%{prj.name}/src/pch.cpp"

	files
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.h",
	}

	includedirs
	{
		"%{prj.name}/src",

		"ChessEngine/vendor/GLFW/include",
		"ChessEngine/vendor/glm",
		"ChessEngine/vendor/stb",
		"ChessEngine/vendor/imgui",
		"%{VULKAN_SDK}/Include"
	}

	links
	{
		"GLFW",
		"glm",
		"ImGui"
	}

	libdirs
	{
		"%{VULKAN_SDK}/Lib"
	}

	filter "system:windows"
		links "vulkan-1"

	filter "system:linux"
		links "vulkan"

	filter "configurations:Debug"
		links "spirv-cross-cored"

	filter "configurations:Release"
		links "spirv-cross-core"

project "HexChess"
	location "%{prj.name}"
	kind "ConsoleApp"

	targetdir "bin/%{cfg.buildcfg}/%{prj.name}"
	objdir "bin-int/%{cfg.buildcfg}/%{prj.name}"

	files
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.h",

		"%{prj.name}/Resources/Shaders/**.vert",
		"%{prj.name}/Resources/Shaders/**.frag",
	}

	includedirs
	{
		"%{prj.name}/src",

		"ChessEngine/src",
		"ChessEngine/vendor/glm",
		"ChessEngine/vendor/stb",
		"ChessEngine/vendor/imgui",
		"%{VULKAN_SDK}/Include"
	}

	links
	{
		"ChessEngine"
	}

	filter "configurations:Release"
		kind "WindowedApp"

	filter "files:**.vert or files:**.frag"
		buildcommands
		{
			"%{VULKAN_SDK}/Bin/glslc -fauto-bind-uniforms %{file.relpath} -o %{file.abspath}.spv"
		}

		buildoutputs
		{
			"%{file.abspath}.spv"
		}
