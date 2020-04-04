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

outputdir = "../%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "../efgl/vendor/GLFW/include"
IncludeDir["Glad"] = "../efgl/vendor/Glad/"


project "efgl"
	location "../efgl/%{prj.name}"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "../%{prj.name}/%{prj.name}/include/efpch.h"
	pchsource "../%{prj.name}/%{prj.name}/src/efpch.cpp"

	files
	{
		"../%{prj.name}/%{prj.name}/include/**.h",
        "../%{prj.name}/%{prj.name}/src/**.cpp",
        "../%{prj.name}/%{prj.name}/src/glad.c"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
        "../%{prj.name}/%{prj.name}/src",
        "../%{prj.name}/%{prj.name}/include",
		"%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
	}

	links 
	{ 
        "../%{prj.name}/vendor/bin/glfw3.lib",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"EF_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
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

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"../efgl/%{prj.name}/src/**.h",
		"../efgl/%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"efgl/src",
		"../efgl/vendor"
	}

	links
	{
		"efgl"
	}

	filter "system:windows"
		systemversion "latest"
		
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