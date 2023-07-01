#pragma once

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#ifdef TASSEL_PLATFORM_WINDOWS
#include <Windows.h>
#include <commdlg.h>

#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <stb_image.h>

#include <entt.hpp>

#include <yaml-cpp/yaml.h>

#include <ImGuizmo.h>

#include <box2d/box2d.h>
#endif // TASSEL_PLATFORM_WINDOWS


#ifdef TASSEL_PLATFORM_WINDOWS
	#ifdef TASSEL_DYNAMIC_LINK
		#ifdef TASSEL_BUILD_DLL
			#define TASSEL_API __declspec(dllexport)
		#else
			#define TASSEL_API __declspec(dllimport)
		#endif // TASSEL_BUILD_DLL
	#else
		#define TASSEL_API
	#endif
#else
	#error Tassel only supports Windows!
#endif // TASSEL_PLATFORM_WINDOWS

#define TASSEL_BIT(x) (1 << x)

#define TASSEL_BIND_EVENT_FUNC(func) std::bind(&func, this, std::placeholders::_1)

#define TASSEL_DEFAULT_WINDOW_WIDTH 1600
#define TASSEL_DEFAULT_WINDOW_HEIGHT 900
