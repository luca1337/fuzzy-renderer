#include <logger.h>

namespace libgraphics_logger
{
	std::shared_ptr<spdlog::logger> Logger::m_core_logger;
	std::shared_ptr<spdlog::logger> Logger::m_client_logger;

	void Logger::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		m_core_logger = spdlog::stdout_color_mt("libgraphics");
		m_core_logger->set_level(spdlog::level::trace);

		m_client_logger = spdlog::stdout_color_st("renderer");
		m_client_logger->set_level(spdlog::level::trace);
	}
}
