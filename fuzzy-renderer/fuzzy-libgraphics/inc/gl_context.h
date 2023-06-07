#pragma once

#include "igraphics_context.h"

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
		auto Data() const -> const ContextData& override { return _contextData; }
		auto Data() -> ContextData& override { return _contextData; }
		[[nodiscard]] auto GetNativeHandle() const -> void* override { return reinterpret_cast<GLFWwindow*>(_glfwNativeWindowHandle); }

	private:

		GLFWwindow* _glfwNativeWindowHandle = {};
		ContextData _contextData = {};

		const int MajorVersion = 4;
		const int MinorVersion = 6;
	};
}
