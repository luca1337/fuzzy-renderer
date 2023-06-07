#include <core.h>
#include <enums.h>
#include <opengl/gl_context.h>
#include <opengl/gl_window.h>
#include <logger.h>

#include <GLFW/glfw3.h>

namespace libgraphics
{
	auto Core::Init(const GraphicsAPI api, int contextWidth, int contextHeight, std::string_view contextTitle) -> void
	{
		// Init logger
		libgraphics_logger::Logger::Init();

		switch (api)
		{
		case GraphicsAPI::OpenGL:
		{
			_graphicsWindow = std::make_shared<GLWindow>();
			_graphicsWindow->Create(contextWidth, contextHeight, contextTitle);
		}
		break;
		case GraphicsAPI::DirectX: /* .. .  .   .    .     .     .    .   .  . .. */ break;
		default: break;
		}
	}

	auto Core::Update(const RenderFunction& renderFunction) -> void
	{
		const auto glfwWindow = reinterpret_cast<GLFWwindow*>(_graphicsWindow->GetNativeHandle()->GetNativeHandle());

		auto previousTime = glfwGetTime();

		while (!glfwWindowShouldClose(glfwWindow))
		{
			auto currentTime = glfwGetTime();
			auto deltaTime = static_cast<float>(currentTime - previousTime);
			previousTime = currentTime;

			_graphicsWindow->Clear();

			if (renderFunction)
			{
				renderFunction(deltaTime);
			}

			_graphicsWindow->SwapBuffers();
		}
	}

	auto Core::GetInstance() -> Core&
	{
		static auto core = Core();
		return core;
	}
}