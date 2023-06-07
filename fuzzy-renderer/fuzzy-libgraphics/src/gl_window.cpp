#include "gl_window.h"
#include "gl_context.h"

#include <GLFW/glfw3.h>

namespace libgraphics
{
	auto GLWindow::Create(const int width, const int height, const std::string_view title) -> void
	{
		_graphicsContext = std::make_shared<GLContext>();
		_graphicsContext->Init(width, height, title);
	}

	auto GLWindow::Destroy() -> void
	{
		_graphicsContext->Shutdown();
	}

	auto GLWindow::ClearBuffer() -> void
	{
		glClearColor(0.4f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	auto GLWindow::SwapBuffers() -> void
	{
		const auto glfwNativeWindowHandle = reinterpret_cast<GLFWwindow*>(_graphicsContext->GetNativeHandle());
		glfwSwapBuffers(glfwNativeWindowHandle);
	}
}
