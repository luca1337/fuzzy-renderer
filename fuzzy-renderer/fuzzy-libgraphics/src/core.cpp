#include <core.h>
#include <enums.h>

#include <logger.h>
#include <opengl/gl_context.h>
#include <opengl/gl_window.h>
#include <opengl/gl_shader.h>
#include <opengl/gl_mesh.h>

#include <GLFW/glfw3.h>

namespace libgraphics
{
	auto Core::Init(const GraphicsAPI api_type, int context_width, int context_height, const std::string_view context_title) -> void
	{
		m_p_impl = new CoreImpl();
		m_p_impl->m_graphics_api = api_type;

		// Init logger
		libgraphics_logger::Logger::Init();

		switch (api_type)
		{
		case GraphicsAPI::OpenGL:
		{
			m_p_impl->m_graphics_window = std::make_shared<GLWindow>();
			m_p_impl->m_graphics_window->Create(context_width, context_height, context_title);

			// compile shaders
			const auto basic_shader = std::make_shared<GLShader>("../fuzzy-libgraphics/shaders/glsl/vertex.glsl", "../fuzzy-libgraphics/shaders/glsl/fragment.glsl");

			m_p_impl->m_main_camera = {};

			std::vector<glm::vec3> vertices =
			{
				// first facet front
				{-1, 1, -1},
				{-1, -1, -1},
				{1, -1, -1},
				{-1, 1, -1},
				{1, 1, -1},
				{1, -1, -1},

				// second facet back
				{-1, 1, 1},
				{-1, -1, 1},
				{1, -1, 1},
				{-1, 1, 1},
				{1, 1, 1},
				{1, -1, 1},

				// third facet left
				{-1, 1, 1},
				{-1, -1, 1},
				{-1, -1, -1},
				{-1, 1, 1},
				{-1, 1, -1},
				{-1, -1, -1},

				// fourth facet right
				{1, 1, 1},
				{1, -1, 1},
				{1, -1, -1},
				{1, 1, 1},
				{1, 1, -1},
				{1, -1, -1},

				// fifth facet up
				{-1, 1, 1},
				{-1, 1, -1},
				{1, 1, -1},
				{-1, 1, 1},
				{1, 1, 1},
				{1, 1, -1},

				// sixth facet down
				{-1, -1, 1},
				{-1, -1, -1},
				{1, -1, -1},
				{-1, -1, 1},
				{1, -1, 1},
				{1, -1, -1}
			};

			std::vector<glm::vec2> uvs =
			{
				{0, 1},
				{0, 0},
				{1, 0},
				{0, 1},
				{1, 1},
				{1, 0},

				{0, 1},
				{0, 0},
				{1, 0},
				{0, 1},
				{1, 1},
				{1, 0},

				{0, 1},
				{0, 0},
				{1, 0},
				{0, 1},
				{1, 1},
				{1, 0},

				{0, 1},
				{0, 0},
				{1, 0},
				{0, 1},
				{1, 1},
				{1, 0},

				{0, 1},
				{0, 0},
				{1, 0},
				{0, 1},
				{1, 1},
				{1, 0},

				{0, 1},
				{0, 0},
				{1, 0},
				{0, 1},
				{1, 1},
				{1, 0},
			};

			std::vector<glm::vec3> normals =
			{
				{0, 0, -1},
				{0, 0, -1},
				{0, 0, -1},
				{0, 0, -1},
				{0, 0, -1},
				{0, 0, -1},

				{0, 0, 1},
				{0, 0, 1},
				{0, 0, 1},
				{0, 0, 1},
				{0, 0, 1},
				{0, 0, 1},

				{-1, 0, 0},
				{-1, 0, 0},
				{-1, 0, 0},
				{-1, 0, 0},
				{-1, 0, 0},
				{-1, 0, 0},

				{1, 0, 0},
				{1, 0, 0},
				{1, 0, 0},
				{1, 0, 0},
				{1, 0, 0},
				{1, 0, 0},

				{0, 1, 0},
				{0, 1, 0},
				{0, 1, 0},
				{0, 1, 0},
				{0, 1, 0},
				{0, 1, 0},

				{0, -1, 0},
				{0, -1, 0},
				{0, -1, 0},
				{0, -1, 0},
				{0, -1, 0},
				{0, -1, 0},
			};

			m_test_cube = std::make_shared<GLMesh>(vertices, normals, uvs);
		}
		break;
		case GraphicsAPI::DirectX: break;
		default: break;
		}
	}

	auto Core::Update(const RenderFunction& render_function) const -> void
	{
		const auto glfw_window = static_cast<GLFWwindow*>(m_p_impl->m_graphics_window->GetNativeHandle()->GetNativeHandle());

		auto previous_time = glfwGetTime();

		while (!glfwWindowShouldClose(glfw_window))
		{
			const auto current_time = glfwGetTime();
			const auto delta_time = current_time - previous_time;
			previous_time = current_time;

			m_p_impl->m_graphics_window->Clear();

			if (glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_2))
			{
				m_p_impl->m_main_camera.RotateByMouse(m_p_impl->m_graphics_window);
			}
			else
			{
				m_p_impl->m_main_camera.Reset();
			}
			m_p_impl->m_main_camera.Animate(m_p_impl->m_graphics_window, delta_time);

			//m_test_cube->Draw(nullptr);

			if (render_function)
			{
				render_function(delta_time);
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