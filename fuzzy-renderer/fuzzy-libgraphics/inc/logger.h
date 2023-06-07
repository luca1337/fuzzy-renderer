#pragma once

#include "framework.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace libgraphics_logger
{
	class Logger
	{
	public:
		LIBGRAPHICS_API static void Init();

		LIBGRAPHICS_API inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return _coreLogger; }
		LIBGRAPHICS_API inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return _clientLogger; }

	private:
		LIBGRAPHICS_API static std::shared_ptr<spdlog::logger> _coreLogger;
		LIBGRAPHICS_API static std::shared_ptr<spdlog::logger> _clientLogger;
	};
}

// Core Logger Macros
#define CX_CORE_TRACE(...)		::libgraphics_logger::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define CX_CORE_INFO(...)		::libgraphics_logger::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define CX_CORE_WARN(...)		::libgraphics_logger::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define CX_CORE_ERROR(...)		::libgraphics_logger::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define CX_CORE_CRITICAL(...)	::libgraphics_logger::Logger::GetCoreLogger()->critical(__VA_ARGS__)
#define CX_CORE_DEBUG(...)		::libgraphics_logger::Logger::GetCoreLogger()->debug(__VA_ARGS__)

// Client Logger Macros
#define CX_TRACE(...)			::libgraphics_logger::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define CX_INFO(...)			::libgraphics_logger::Logger::GetClientLogger()->info(__VA_ARGS__)
#define CX_WARN(...)			::libgraphics_logger::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define CX_ERROR(...)			::libgraphics_logger::Logger::GetClientLogger()->error(__VA_ARGS__)
#define CX_CRIT(...)			::libgraphics_logger::Logger::GetClientLogger()->critical(__VA_ARGS__)
#define CX_DEBUG(...)			::libgraphics_logger::Logger::GetClientLogger()->debug(__VA_ARGS__)