#pragma once
#include "Tassel/Core/Base.h"

namespace Tassel
{
	class TASSEL_API Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_CoreLogger; };
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return m_ClientLogger; };
	private:
		static std::shared_ptr<spdlog::logger> m_CoreLogger;
		static std::shared_ptr<spdlog::logger> m_ClientLogger;
	};
}

#ifdef TASSEL_ENABLE_ASSERT
	#define TASSEL_CORE_ASSERT(x, ...) { if (!(x)) { TASSEL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define TASSEL_ASSERT(x, ...) { if (!(x)) { TASSEL_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define TASSEL_CORE_ASSERT(x, ...)
	#define TASSEL_ASSERT(x, ...)
#endif // TASSEL_ENABLE_ASSERT

#ifdef TASSEL_DEBUG
	#define TASSEL_DEBUG_PRINT(...) TASSEL_CORE_TRACE(__VA_ARGS__)
#else
	#define TASSEL_DEBUG_PRINT(...)
#endif // TASSEL_DEBUG

// Core log macros
#define TASSEL_CORE_TRACE(...)     ::Tassel::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define TASSEL_CORE_DEBUG(...)     ::Tassel::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define TASSEL_CORE_INFO(...)      ::Tassel::Log::GetCoreLogger()->info(__VA_ARGS__)
#define TASSEL_CORE_WARN(...)      ::Tassel::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define TASSEL_CORE_ERROR(...)     ::Tassel::Log::GetCoreLogger()->error(__VA_ARGS__)
#define TASSEL_CORE_CRITICAL(...)  ::Tassel::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define TASSEL_TRACE(...)          ::Tassel::Log::GetClientLogger()->trace(__VA_ARGS__)
//#define TASSEL_DEBUG(...)          ::Tassel::Log::GetClientLogger()->debug(__VA_ARGS__)
#define TASSEL_INFO(...)           ::Tassel::Log::GetClientLogger()->info(__VA_ARGS__)
#define TASSEL_WARN(...)           ::Tassel::Log::GetClientLogger()->warn(__VA_ARGS__)
#define TASSEL_ERROR(...)          ::Tassel::Log::GetClientLogger()->error(__VA_ARGS__)
#define TASSEL_CRITICAL(...)       ::Tassel::Log::GetClientLogger()->critical(__VA_ARGS__)
