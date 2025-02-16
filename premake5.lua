include "premake/solution_items.lua"

VULKAN_SDK = os.getenv("VULKAN_SDK")

workspace "HexChess"
	startproject "HexChess"

	architecture "x86_64"
	language "C++"
	cppdialect "C++23"

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

	targetdir "bin/%{prj.name}"
	objdir "bin-int/%{prj.name}"

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
		"%{VULKAN_SDK}/Include"
	}

	links
	{
		"GLFW"
	}

	libdirs
	{
		"%{VULKAN_SDK}/Lib"
	}

	filter "system:windows"
		links "vulkan-1"

	filter "system:linux"
		links "vulkan"

project "HexChess"
	location "%{prj.name}"
	kind "ConsoleApp"

	targetdir "bin/%{prj.name}"
	objdir "bin-int/%{prj.name}"

	files
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.h",
	}

	includedirs
	{
		"%{prj.name}/src",
		"ChessEngine/src"
	}

	links
	{
		"ChessEngine"
	}
