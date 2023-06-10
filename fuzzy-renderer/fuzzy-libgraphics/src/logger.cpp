#include <logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace libgraphics::logger
{
	std::shared_ptr<spdlog::logger> Logger::m_core_logger;
	std::shared_ptr<spdlog::logger> Logger::m_client_logger;

	void Logger::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		m_core_logger = spdlog::stdout_color_mt("ENGINE");
		m_core_logger->set_level(spdlog::level::trace);

		m_client_logger = spdlog::stdout_color_st("CLIENT");
		m_client_logger->set_level(spdlog::level::trace);
	}
}
