-- Pretty much copied from the cherno's version
-- at https://github.com/TheCherno/Hazel/blob/master/premake5.lua
workspace "efgl"
        architecture "x86_64"
        startproject "efgl"
        filename "efgl"
        location "../"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

	outputdir = "/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	IncludeDir = {}
	IncludeDir["GLFW"] = "../vendor/GLFW/include"
	IncludeDir["Glad"] = "../vendor/Glad/include"
	IncludeDir["stb"]  = "../vendor/stb"
	IncludeDir["glm"]  = "../vendor/glm"
	IncludeDir["spdlog"] = "../vendor/spdlog/include"
	IncludeDir["assimp"] = "../vendor/assimp/include"
	IncludeDir["ImGui"] = "../vendor/imgui"
        IncludeDir["tracy"] = "../vendor/tracy/common/"

	group "Dependencies"
		include "../vendor/GLFW"
		include "../vendor/Glad"
		include "../vendor/assimp"
		include "../vendor/imgui"
	group ""

	project "efgl"
		location "../efgl/"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
                staticruntime "on"

		targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
		objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

		pchheader "efpch.h"
		pchsource "../%{prj.name}/src/efpch.cpp"

		files
		{
			"../efgl/src/**.h",
			"../efgl/src/**.cpp",
			"../vendor/stb/**.h",
			"../vendor/stb/**.cpp",
			"../vendor/tracy/TracyClient.cpp",
			"../vendor/Glad/src/glad.c"
		}

		defines
		{
			"_CRT_SECURE_NO_WARNINGS"
		}

		includedirs
		{
			"../%{prj.name}/src/",
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.Glad}",
			"%{IncludeDir.stb}",
			"%{IncludeDir.glm}",
			"%{IncludeDir.spdlog}",
			"%{IncludeDir.assimp}",
			"%{IncludeDir.ImGui}",
                        "%{IncludeDir.tracy}"
		}

		links 
		{ 
			"GLFW",
			"Glad",
			"opengl32.lib",
			"assimp",
			"ImGui"
		}
		
		filter { 'files:../vendor/Glad/src/glad.c' }
			flags { 'NoPCH' }
		
		filter { 'files:../vendor/stb/**.h' }
			flags { 'NoPCH' }

		filter { 'files:../vendor/stb/**.cpp' }
			flags { 'NoPCH' }

        filter { 'files:../vendor/tracy/TracyClient.cpp' }
			flags { 'NoPCH' }

		filter "system:windows"
			systemversion "latest"

			defines
			{
				"EF_BUILD_DLL",
				"GLFW_INCLUDE_NONE",
				"RENDERAPI_OGL"
			}

        filter "system:linux"
            systemversion "latest"

            defines 
            {
                "EF_BUILD_DLL",
                "GLFW_INCLUDE_NONE",
                "RENDERAPI_OGL"
            } 

		filter "configurations:Debug"
			defines "EF_DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "EF_RELEASE"
			runtime "Release"
			optimize "on"

		filter "configurations:Dist"
			defines "EF_DIST"
			runtime "Release"
			optimize "on"
