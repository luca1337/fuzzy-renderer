#include <core.h>
#include <enums.h>
#include <iostream>
#include <opengl/gl_context.h>
#include <opengl/gl_window.h>
#include <logger.h>
#include <opengl/gl_shader.h>

#include <GLFW/glfw3.h>

namespace libgraphics
{
	auto Core::Init(const GraphicsAPI api, int contextWidth, int contextHeight, std::string_view contextTitle) -> void
	{
		m_p_impl = new CoreImpl();
		m_p_impl->m_graphics_api = api;

		// Init logger
		libgraphics_logger::Logger::Init();

		switch (api)
		{
		case GraphicsAPI::OpenGL:
		{
			m_p_impl->m_graphics_window = std::make_shared<GLWindow>();
			m_p_impl->m_graphics_window->Create(contextWidth, contextHeight, contextTitle);

			// compile shaders
			auto basic_shader = libgraphics::GLShader("../fuzzy-libgraphics/shaders/glsl/vertex.glsl", "../fuzzy-libgraphics/shaders/glsl/fragment.glsl");

			m_p_impl->m_main_camera = {};
		}
		break;
		case GraphicsAPI::DirectX: break;
		default: break;
		}
	}

	auto Core::Update(const RenderFunction& renderFunction) const -> void
	{
		const auto glfwWindow = static_cast<GLFWwindow*>(m_p_impl->m_graphics_window->GetNativeHandle()->GetNativeHandle());

		auto previous_time = glfwGetTime();

		while (!glfwWindowShouldClose(glfwWindow))
		{
			const auto current_time = glfwGetTime();
			const auto delta_time = current_time - previous_time;
			previous_time = current_time;

			m_p_impl->m_graphics_window->Clear();

			if (glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_2))
			{
				m_p_impl->m_main_camera.RotateByMouse(m_p_impl->m_graphics_window);
			}
			else
			{
				m_p_impl->m_main_camera.Reset();
			}
			m_p_impl->m_main_camera.Animate(m_p_impl->m_graphics_window, delta_time);

			if (renderFunction)
			{
				renderFunction(delta_time);
			}

			m_p_impl->m_graphics_window->SwapBuffers();
		}

		m_p_impl->m_graphics_window->Destroy();
	}

	auto Core::GetInstance() -> Core&
	{
		static auto core = Core();
		return core;
	}
}