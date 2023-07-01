project "Tassel-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Tassel/src",
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
		"Tassel"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"TASSEL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "TASSEL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "TASSEL_RELEASE"
		runtime "Release"
		optimize "on"
