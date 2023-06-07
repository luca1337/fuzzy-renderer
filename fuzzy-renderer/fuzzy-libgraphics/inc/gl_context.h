#pragma once

#include "framework.h"
#include "igraphics_context.h"

struct GLFWwindow;

namespace libgraphics
{
	class LIBGRAPHICS_API GLContext : public IGraphicsContext
	{
	public:
		~GLContext() = default;
		GLContext() = default;
		GLContext(const GLContext&) = delete;
		GLContext(GLContext&&) = delete;

		auto Init(const int width, const int height, const std::string_view title) -> void override;
		auto Shutdown() -> void override;

		ContextData ContextData = {};
	private:

		GLFWwindow* _glfwNativeWindowHandle = {};

		const int MajorVersion = 4;
		const int MinorVersion = 6;
	};
}
