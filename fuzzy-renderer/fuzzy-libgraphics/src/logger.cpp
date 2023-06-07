#include "logger.h"

namespace libgraphics_logger
{
	std::shared_ptr<spdlog::logger> Logger::_coreLogger;
	std::shared_ptr<spdlog::logger> Logger::_clientLogger;

	void Logger::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		_coreLogger = spdlog::stdout_color_mt("libgraphics");
		_coreLogger->set_level(spdlog::level::trace);

		_clientLogger = spdlog::stdout_color_st("renderer");
		_clientLogger->set_level(spdlog::level::trace);
	}
}
