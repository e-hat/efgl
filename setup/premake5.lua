-- Pretty much copied from the cherno's version 
-- at https://github.com/TheCherno/Hazel/blob/master/premake5.lua
workspace "efgl"
	architecture "x86_64"
    startproject "Sandbox"
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

	group "Dependencies"
		include "../efgl/vendor/GLFW"
		include "../efgl/vendor/Glad"
		include "../efgl/vendor/assimp"
		include "../efgl/vendor/imgui"
	group ""

	project "efgl"
		location "../efgl/"
		kind "StaticLib"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"

		targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
		objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

		pchheader "efpch.h"
		pchsource "../%{prj.name}/src/efpch.cpp"

		files
		{
			"../%{prj.name}/src/**",
			"../vendor/stb/**.h",
			"../vendor/stb/**.cpp"
		}

		defines
		{
			"_CRT_SECURE_NO_WARNINGS",
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
			"%{IncludeDir.ImGui}"
		}

		links 
		{ 
			"GLFW",
			"Glad",
			"opengl32.lib",
			"assimp",
			"ImGui"
		}
		
		filter { 'files:../efgl/src/glad.c' }
			flags { 'NoPCH' }
		
		filter { 'files:../vendor/stb/**.h' }
			flags { 'NoPCH' }

		filter { 'files:../vendor/stb/**.cpp' }
			flags { 'NoPCH' }

		filter "system:windows"
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

	project "Sandbox"
		location "../Sandbox"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"

		targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
		objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

		files
		{
			"../%{prj.name}/src/**"
		}

		excludes
		{
			"../%{prj.name}/src/examples/**"
		}

		includedirs
		{
			"../efgl/src",
			"../vendor",
			"../Sandbox/src",
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.Glad}",
			"%{IncludeDir.glm}",
			"%{IncludeDir.stb}",
			"%{IncludeDir.spdlog}",
			"%{IncludeDir.assimp}",
			"%{IncludeDir.ImGui}"
		}

		links
		{
			"efgl",
			"ImGui"
		}

		filter "system:windows"
			systemversion "latest"

			defines 
			{
				"RENDERAPI_OGL",
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