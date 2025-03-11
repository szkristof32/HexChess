function file_exists(name)
   local f=io.open(name,"r")
   if f~=nil then io.close(f) return true else return false end
end

group "Dependencies"

project "GLFW"
	location "ChessEngine/vendor/GLFW"

	kind "StaticLib"
	language "C"
	staticruntime "off"
	warnings "off"

	targetdir ("bin/%{cfg.buildcfg}/%{prj.name}")
	objdir ("bin-int/%{cfg.buildcfg}/%{prj.name}")

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

project "glm"
	location "ChessEngine/vendor/glm"

	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	
	targetdir ("bin/%{cfg.buildcfg}/%{prj.name}")
	objdir ("bin-int/%{cfg.buildcfg}/%{prj.name}")
	
	files
	{
		"ChessEngine/vendor/glm/glm/**.cpp",
		"ChessEngine/vendor/glm/glm/**.hpp",
		"ChessEngine/vendor/glm/glm/**.inl"
	}
	
	includedirs
	{
		"ChessEngine/vendor/glm"
	}
	
project "ImGui"
	location "ChessEngine/vendor/imgui"

	kind "StaticLib"
	language "C++"
    staticruntime "off"

	targetdir ("bin/%{cfg.buildcfg}/%{prj.name}")
	objdir ("bin-int/%{cfg.buildcfg}/%{prj.name}")

	files
	{
		"ChessEngine/vendor/imgui/imconfig.h",
		"ChessEngine/vendor/imgui/imgui.h",
		"ChessEngine/vendor/imgui/imgui.cpp",
		"ChessEngine/vendor/imgui/imgui_draw.cpp",
		"ChessEngine/vendor/imgui/imgui_internal.h",
		"ChessEngine/vendor/imgui/imgui_tables.cpp",
		"ChessEngine/vendor/imgui/imgui_widgets.cpp",
		"ChessEngine/vendor/imgui/imstb_rectpack.h",
		"ChessEngine/vendor/imgui/imstb_textedit.h",
		"ChessEngine/vendor/imgui/imstb_truetype.h",
		"ChessEngine/vendor/imgui/imgui_demo.cpp"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++20"

	filter "system:linux"
		pic "On"
		cppdialect "C++20"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

project "assimp"
	location "HexChess/vendor/assimp"

	kind "SharedLib"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/%{cfg.buildcfg}/%{prj.name}")
	objdir ("bin-int/%{cfg.buildcfg}/%{prj.name}")

	files
	{
		"HexChess/vendor/assimp/code/**.cpp",
		"HexChess/vendor/assimp/code/**.c",
		"HexChess/vendor/assimp/code/**.h",

		"HexChess/vendor/assimp/contrib/zlib/*.c",
		"HexChess/vendor/assimp/contrib/zlib/*.h",
		"HexChess/vendor/assimp/contrib/zip/src/**.c",
		"HexChess/vendor/assimp/contrib/zip/src/**.h",
		"HexChess/vendor/assimp/contrib/poly2tri/**.cc",
		"HexChess/vendor/assimp/contrib/poly2tri/**.h",
		"HexChess/vendor/assimp/contrib/clipper/**.cpp",
		"HexChess/vendor/assimp/contrib/clipper/**.h",
		"HexChess/vendor/assimp/contrib/Open3DGC/**.cpp",
		"HexChess/vendor/assimp/contrib/Open3DGC/**.inl",
		"HexChess/vendor/assimp/contrib/Open3DGC/**.h",
		"HexChess/vendor/assimp/contrib/openddlparser/**.cpp",
		"HexChess/vendor/assimp/contrib/openddlparser/**.h",
		"HexChess/vendor/assimp/contrib/pugixml/src/**.hpp",
		"HexChess/vendor/assimp/contrib/unzip/**.c",
		"HexChess/vendor/assimp/contrib/unzip/**.h",
	}

	removefiles
	{
		"HexChess/vendor/assimp/code/AssetLib/IFC/IFCReaderGen_4.cpp",
		"HexChess/vendor/assimp/code/AssetLib/IFC/IFCReaderGen_4.h",
	}

	includedirs
	{
		"HexChess/vendor/assimp",
		"HexChess/vendor/assimp/code",
		"HexChess/vendor/assimp/code/include",
		"HexChess/vendor/assimp/include",

		"HexChess/vendor/assimp/contrib",
		"HexChess/vendor/assimp/contrib/pugixml/src",
		"HexChess/vendor/assimp/contrib/utf8cpp/source",
		"HexChess/vendor/assimp/contrib/rapidjson/include",
		"HexChess/vendor/assimp/contrib/unzip",
		"HexChess/vendor/assimp/contrib/openddlparser/include",
		"HexChess/vendor/assimp/contrib/zlib",
	}

	defines
	{
		"ASSIMP_BUILD_DLL_EXPORT",
		"ASSIMP_BUILD_NO_M3D_IMPORTER",
		"ASSIMP_BUILD_NO_M3D_EXPORTER",
		"ASSIMP_BUILD_NO_VRML_IMPORTER",
		"ASSIMP_BUILD_NO_C4D_IMPORTER",
		"ASSIMP_BUILD_NO_USD_IMPORTER",
		"MINIZ_USE_UNALIGNED_LOADS_AND_STORES=0",
		"ASSIMP_IMPORTER_GLTF_USE_OPEN3DGC=1",
		"RAPIDJSON_HAS_STDSTRING=1",
		"RAPIDJSON_NOMEMBERITERATORCLASS",
		"OPENDDLPARSER_BUILD",
		"assimp_EXPORTS"
	}

	defines
	{
		"_CRT_SECURE_NO_DEPRECATE",
		"_CRT_NONSTDC_NO_DEPRECATE"
	}

	filter "action:vs*"
        disablewarnings
		{
			"4127",
			"4131",
			"4244"
		}

	postbuildcommands
	{
		"{MKDIR} %{wks.location}/bin/%{cfg.buildcfg}/HexChess",
		"{COPYFILE} %{cfg.buildtarget.relpath} %{wks.location}/bin/%{cfg.buildcfg}/HexChess/%{cfg.buildtarget.name}"
	}

	newaction {
		trigger = "assimp",
		description = "assimp generation",
		execute = function()
			if not file_exists("HexChess/vendor/assimp/include/assimp/config.h") then
				local file = io.open("HexChess/vendor/assimp/include/assimp/config.h", "w+")
				for line in io.lines("HexChess/vendor/assimp/include/assimp/config.h.in") do
					line = line:gsub("(#cmakedefine).+", "")
					file:write(line .. "\n")
				end
			end

			if not file_exists("HexChess/vendor/assimp/include/assimp/revision.h") then
				local file = io.open("HexChess/vendor/assimp/include/assimp/revision.h", "w+")
				local handle = io.popen("git rev-parse --abbrev-ref HEAD")
				local gitBranch = handle:read()
				handle:close()
				local handle = io.popen("git rev-parse --short=8 HEAD")
				local gitCommitHash = handle:read()
				handle:close()
				for line in io.lines("HexChess/vendor/assimp/include/assimp/revision.h.in") do
					line = line:gsub("@CMAKE_SHARED_LIBRARY_PREFIX@", "")
					line = line:gsub("@LIBRARY_SUFFIX@", "")
					line = line:gsub("@CMAKE_DEBUG_POSTFIX@", "")
					line = line:gsub("@ASSIMP_VERSION_MAJOR@", "5") -- Maybe can be read from the cmake file
					line = line:gsub("@ASSIMP_VERSION_MINOR@", "4")
					line = line:gsub("@ASSIMP_VERSION_PATCH@", "3")
					line = line:gsub("@ASSIMP_PACKAGE_VERSION@", "0")
					line = line:gsub("@GIT_BRANCH@", gitBranch)
					line = line:gsub("@GIT_COMMIT_HASH@", gitCommitHash)
					file:write(line .. "\n")
				end
			end

			if not file_exists("HexChess/vendor/assimp/contrib/zlib/zconf.h") then
				local file = io.open("HexChess/vendor/assimp/contrib/zlib/zconf.h", "w+")
				for line in io.lines("HexChess/vendor/assimp/contrib/zlib/zconf.h.in") do
					file:write(line .. "\n")
				end
			end
		end
	}

group ""
