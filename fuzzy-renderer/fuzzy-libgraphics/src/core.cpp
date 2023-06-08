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
		_pImpl = new CoreImpl();
		_pImpl->_graphicsApi = api;

		// Init logger
		libgraphics_logger::Logger::Init();

		switch (api)
		{
		case GraphicsAPI::OpenGL:
		{
			_pImpl->_graphicsWindow = std::make_unique<GLWindow>();
			_pImpl->_graphicsWindow->Create(contextWidth, contextHeight, contextTitle);
		}
		break;
		case GraphicsAPI::DirectX: /* .. .  .   .    .     .     .    .   .  . .. */ break;
		default: break;
		}
	}

	auto Core::Update(const RenderFunction& renderFunction) -> void
	{
		const auto glfwWindow = reinterpret_cast<GLFWwindow*>(_pImpl->_graphicsWindow->GetNativeHandle()->GetNativeHandle());

		auto previousTime = glfwGetTime();

		while (!glfwWindowShouldClose(glfwWindow))
		{
			auto currentTime = glfwGetTime();
			auto deltaTime = static_cast<float>(currentTime - previousTime);
			previousTime = currentTime;

			_pImpl->_graphicsWindow->Clear();

			if (renderFunction)
			{
				renderFunction(deltaTime);
			}

			_pImpl->_graphicsWindow->SwapBuffers();
		}
	}

	auto Core::GetInstance() -> Core&
	{
		static auto core = Core();
		return core;
	}
}