#pragma once

#include "interfaces/igraphics_context.h"

struct GLFWwindow;

namespace libgraphics
{
	class LIBGRAPHICS_API GLContext : public IGraphicsContext
	{
	public:
		GLContext() = default;
		GLContext(const GLContext&) = delete;
		GLContext(GLContext&&) = delete;

		auto Init(const int width, const int height, const std::string_view title) -> void override;
		auto Shutdown() -> void override;
		auto Data() const -> const ContextData& override { return m_context_data; }
		auto Data() -> ContextData& override { return m_context_data; }
		[[nodiscard]] auto GetNativeHandle() const -> void* override { return reinterpret_cast<GLFWwindow*>(m_glfw_native_window_handle); }

	private:

		GLFWwindow* m_glfw_native_window_handle = {};
		ContextData m_context_data = {};

		const int MajorVersion = 4;
		const int MinorVersion = 6;
	};
}
