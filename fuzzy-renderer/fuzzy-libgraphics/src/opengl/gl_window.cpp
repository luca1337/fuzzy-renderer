#include <opengl/gl_window.h>
#include <opengl/gl_context.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace libgraphics
{
	auto GLWindow::Create(const int width, const int height, const std::string_view title) -> void
	{
		m_graphics_context = std::make_shared<GLContext>();
		m_graphics_context->Init(width, height, title);
	}

	auto GLWindow::Destroy() -> void
	{
		m_graphics_context->Shutdown();
	}

	auto GLWindow::Clear() -> void
	{
		glClearColor(0.4f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	auto GLWindow::SwapBuffers() -> void
	{
		const auto glfwNativeWindowHandle = reinterpret_cast<GLFWwindow*>(m_graphics_context->GetNativeHandle());

		glfwSwapBuffers(glfwNativeWindowHandle);
		glfwPollEvents();
	}
}
