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
		glClearColor(m_clear_color.r, m_clear_color.g, m_clear_color.b, m_clear_color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	auto GLWindow::SwapBuffers() -> void
	{
		const auto glfw_native_window_handle = static_cast<GLFWwindow*>(m_graphics_context->GetNativeHandle());

		glfwSwapBuffers(glfw_native_window_handle);
		glfwPollEvents();
	}

	auto GLWindow::SetClearColor(const Color& color) -> void
	{
		m_clear_color = color;
	}
}
