#pragma once

#include "framework.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace libgraphics::logger
{
	class Logger
	{
	public:
		LIBGRAPHICS_API static void Init();

		LIBGRAPHICS_API inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_core_logger; }
		LIBGRAPHICS_API inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return m_client_logger; }

	private:
		LIBGRAPHICS_API static std::shared_ptr<spdlog::logger> m_core_logger;
		LIBGRAPHICS_API static std::shared_ptr<spdlog::logger> m_client_logger;
	};
}

// Core Logger Macros
#define CX_CORE_TRACE(...)		::libgraphics::logger::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define CX_CORE_INFO(...)		::libgraphics::logger::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define CX_CORE_WARN(...)		::libgraphics::logger::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define CX_CORE_ERROR(...)		::libgraphics::logger::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define CX_CORE_CRITICAL(...)	::libgraphics::logger::Logger::GetCoreLogger()->critical(__VA_ARGS__)
#define CX_CORE_DEBUG(...)		::libgraphics::logger::Logger::GetCoreLogger()->debug(__VA_ARGS__)

// Client Logger Macros
#define CX_TRACE(...)			::libgraphics::logger::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define CX_INFO(...)			::libgraphics::logger::Logger::GetClientLogger()->info(__VA_ARGS__)
#define CX_WARN(...)			::libgraphics::logger::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define CX_ERROR(...)			::libgraphics::logger::Logger::GetClientLogger()->error(__VA_ARGS__)
#define CX_CRITICAL(...)		::libgraphics::logger::Logger::GetClientLogger()->critical(__VA_ARGS__)
#define CX_DEBUG(...)			::libgraphics::logger::Logger::GetClientLogger()->debug(__VA_ARGS__)