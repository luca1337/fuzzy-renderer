#include "gl_context.h"
#include "logger.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace libgraphics
{
	auto FramebufferSizeCallback(GLFWwindow* window, const int width, const int height) -> void
	{
		auto& windowPtr = *static_cast<GLContext*>(glfwGetWindowUserPointer(window));
		windowPtr.Data().width = width;
		windowPtr.Data().height = height;
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

		_glfwNativeWindowHandle = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
		if (!_glfwNativeWindowHandle) 
		{
			CX_CORE_ERROR("Unable to create glfw3 Window");
			glfwTerminate();
			return;
		}

		// Imposta il contesto OpenGL sulla finestra
		glfwMakeContextCurrent(_glfwNativeWindowHandle);
		glfwSetWindowUserPointer(_glfwNativeWindowHandle, this);
		glfwSetFramebufferSizeCallback(_glfwNativeWindowHandle, FramebufferSizeCallback);

		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		{
			CX_CORE_ERROR("Unable to load opengl/glad symbols gladLoadGLLoader()");
			glfwDestroyWindow(_glfwNativeWindowHandle);
			glfwTerminate();
			return;
		}

		glViewport(0, 0, width, height);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		CX_CORE_INFO("OpenGL Context initialized!");
	}

	auto GLContext::Shutdown() -> void
	{
		CX_CORE_TRACE("Shutting down GL Context");

		glfwDestroyWindow(_glfwNativeWindowHandle);
		glfwTerminate();
	}
}
