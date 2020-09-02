-- Pretty much copied from the cherno's version 
-- at https://github.com/TheCherno/Hazel/blob/master/premake5.lua
workspace "efgl"
	architecture "x86_64"
    startproject "Sandbox"
    filename "efgl"
    location "../efgl/"

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

	-- Include directories relative to root folder (solution directory)
	IncludeDir = {}
	IncludeDir["GLFW"] = "../efgl/vendor/GLFW/include"
	IncludeDir["Glad"] = "../efgl/vendor/Glad/include"
	IncludeDir["stb"]  = "../efgl/vendor/stb"
	IncludeDir["glm"]  = "../efgl/vendor/glm"
	IncludeDir["spdlog"] = "../efgl/vendor/spdlog/include"
	IncludeDir["assimp"] = "../efgl/vendor/assimp/include"
	IncludeDir["ImGui"] = "../efgl/vendor/imgui"

	group "Dependencies"
		include "../efgl/vendor/GLFW"
		include "../efgl/vendor/Glad"
		include "../efgl/vendor/assimp"
		include "../efgl/vendor/imgui"
	group ""

	project "efgl"
		location "../efgl/%{prj.name}"
		kind "StaticLib"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"

		targetdir ("../efgl/bin/" .. outputdir .. "/%{prj.name}")
		objdir ("../efgl/bin-int/" .. outputdir .. "/%{prj.name}")

		pchheader "efpch.h"
		pchsource "../efgl/%{prj.name}/src/efgl/efpch.cpp"

		files
		{
			"../efgl/%{prj.name}/src/**",
			"../efgl/vendor/stb/**.h",
			"../efgl/vendor/stb/**.cpp"
		}

		defines
		{
			"_CRT_SECURE_NO_WARNINGS",
		}

		includedirs
		{
			"../efgl/%{prj.name}/src/efgl",
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
		
		filter { 'files:../efgl/efgl/src/efgl/glad.c' }
			flags { 'NoPCH' }
		
		filter { 'files:../efgl/vendor/stb/**.h' }
			flags { 'NoPCH' }

		filter { 'files:../efgl/vendor/stb/**.cpp' }
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
		location "../efgl/%{prj.name}"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"

		targetdir ("../efgl/bin/" .. outputdir .. "/%{prj.name}")
		objdir ("../efgl/bin-int/" .. outputdir .. "/%{prj.name}")

		files
		{
			"../efgl/%{prj.name}/src/**"
		}

		excludes
		{
			"../efgl/%{prj.name}/src/examples/**"
		}

		includedirs
		{
			"../efgl/efgl/src",
			"../efgl/efgl/src/efgl",
			"../efgl/vendor",
			"../efgl/Sandbox/src",
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