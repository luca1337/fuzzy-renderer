#include "gl_context.h"

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
			//todo: gestione errore
			return;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MajorVersion);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MinorVersion);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Crea la finestra GLFW
		_glfwNativeWindowHandle = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
		if (!_glfwNativeWindowHandle) 
		{
			//todo: gestione errore
			glfwTerminate();
			return;
		}

		// Imposta il contesto OpenGL sulla finestra
		glfwMakeContextCurrent(_glfwNativeWindowHandle);
		glfwSetWindowUserPointer(_glfwNativeWindowHandle, this);
		glfwSetFramebufferSizeCallback(_glfwNativeWindowHandle, FramebufferSizeCallback);

		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		{
			//todo: gestione errore
			return;
		}

		glViewport(0, 0, width, height);
	}

	auto GLContext::Shutdown() -> void
	{
		// todo: implement shutdown
	}
}
