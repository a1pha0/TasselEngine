project "Tassel"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "Tassel/Core/Base.h"
	pchsource "src/Tassel/Core/Base.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"%{IncludeDir.glm}/glm/**.hpp",
		"%{IncludeDir.glm}/glm/**.inl",
		"%{IncludeDir.stb}/**.h",
		"%{IncludeDir.stb}/**.cpp",
		"%{IncludeDir.ImGuizmo}/ImGuizmo.h",
		"%{IncludeDir.ImGuizmo}/ImGuizmo.cpp",
	}

	includedirs
	{
		"src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.box2d}",
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"box2d",
		"opengl32.lib"
	}
	
	filter "files:thirdparty/**.cpp"
		flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"
		defines
		{
			"TASSEL_PLATFORM_WINDOWS",
			"TASSEL_BUILD_DLL",
			"TASSEL_ENABLE_ASSERT",
			"GLFW_INCLUDE_NONE",
			"GLFW_EXPOSE_NATIVE_WIN32",
			"YAML_CPP_STATIC_DEFINE",
		}

	filter "configurations:Debug"
		defines "TASSEL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "TASSEL_RELEASE"
		runtime "Release"
		optimize "on"
