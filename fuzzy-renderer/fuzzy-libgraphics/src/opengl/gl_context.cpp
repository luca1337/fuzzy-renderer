#include <opengl/gl_context.h>
#include <logger.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace libgraphics
{
	auto framebuffer_size_callback(GLFWwindow* window, const int width, const int height) -> void
	{
		auto& windowPtr = *static_cast<GLContext*>(glfwGetWindowUserPointer(window));
		windowPtr.Data().m_width = width;
		windowPtr.Data().m_height = height;
		glViewport(0, 0, width, height);
	}

	auto GLContext::Init(const int width, const int height, const std::string_view title) -> void
	{
		if (!glfwInit())
		{
			CX_CORE_ERROR("Unable to initialize glfw3");
			return;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MajorVersion);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MinorVersion);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		m_glfw_native_window_handle = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
		if (!m_glfw_native_window_handle) 
		{
			CX_CORE_ERROR("Unable to create glfw3 Window");
			glfwTerminate();
			return;
		}

		// Imposta il contesto OpenGL sulla finestra
		glfwMakeContextCurrent(m_glfw_native_window_handle);
		glfwSetWindowUserPointer(m_glfw_native_window_handle, this);
		glfwSetFramebufferSizeCallback(m_glfw_native_window_handle, framebuffer_size_callback);

		if (!gladLoadGL(glfwGetProcAddress))
		{
			CX_CORE_ERROR("Unable to load opengl/glad symbols gladLoadGL()");
			glfwDestroyWindow(m_glfw_native_window_handle);
			glfwTerminate();
			return;
		}

		glViewport(0, 0, width, height);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		m_context_data = { width, height };

		CX_CORE_INFO("OpenGL Context initialized!");
	}

	auto GLContext::Shutdown() -> void
	{
		CX_CORE_TRACE("Shutting down GL Context");

		glfwDestroyWindow(m_glfw_native_window_handle);
		glfwTerminate();
	}
}
