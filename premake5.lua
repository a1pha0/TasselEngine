workspace "Tassel"
	architecture "x64"
	startproject "Tassel-Editor"
	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["spdlog"]   = "%{wks.location}/Tassel/thirdparty/spdlog/include"
IncludeDir["GLFW"]	   = "%{wks.location}/Tassel/thirdparty/GLFW/include"
IncludeDir["Glad"]	   = "%{wks.location}/Tassel/thirdparty/Glad/include"
IncludeDir["ImGui"]	   = "%{wks.location}/Tassel/thirdparty/ImGui"
IncludeDir["glm"]	   = "%{wks.location}/Tassel/thirdparty/glm"
IncludeDir["stb"]	   = "%{wks.location}/Tassel/thirdparty/stb"
IncludeDir["entt"]	   = "%{wks.location}/Tassel/thirdparty/entt"
IncludeDir["yaml_cpp"] = "%{wks.location}/Tassel/thirdparty/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Tassel/thirdparty/ImGuizmo"
IncludeDir["box2d"] = "%{wks.location}/Tassel/thirdparty/box2d/include"

group "Dependencies"
	include "Tassel/thirdparty/GLFW"
	include "Tassel/thirdparty/Glad"
	include "Tassel/thirdparty/ImGui"
	include "Tassel/thirdparty/yaml-cpp"
	include "Tassel/thirdparty/box2d"
group ""

include "Tassel"
include "Tassel-Editor"
include "SandBox"
