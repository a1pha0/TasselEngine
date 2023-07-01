#include "Tassel/Core/Base.h"
#include "Tassel/Core/Log.h"

namespace Tassel
{
	std::shared_ptr<spdlog::logger> Log::m_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::m_ClientLogger;
	
	void Log::Init()
	{
		//std::string pattern = "%^[%Y-%m-%d %T] [%n] [%l] : %v%$";
		std::string pattern = "%^[%T] [%n] [%l] : %v%$";

		m_CoreLogger = spdlog::stdout_color_mt("Core");
		m_CoreLogger->set_pattern(pattern);
		m_CoreLogger->set_level(spdlog::level::trace);

		m_ClientLogger = spdlog::stdout_color_mt("Client");
		m_ClientLogger->set_pattern(pattern);
		m_ClientLogger->set_level(spdlog::level::trace);
	}
}
