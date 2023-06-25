#include <core.h>
#include <enums.h>
#include <filesystem>

#include <logger.h>
#include <resource_manager.h>
#include <opengl/gl_context.h>
#include <opengl/gl_shader.h>
#include <opengl/gl_skybox.h>
#include <opengl/gl_window.h>
#include <opengl/model.h>

#include <gui_utils.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

#include "gui/windows/gui_window_stats.h"
#include "rendering/light.h"

namespace libgraphics
{
	auto Core::Init(const GraphicsAPI api_type, const int context_width, const int context_height, const std::string_view context_title) -> void
	{
		m_p_impl = new CoreImpl();
		m_p_impl->m_graphics_api = api_type;

		// Init logger
		libgraphics::logger::Logger::Init();

		switch (api_type)
		{
		case GraphicsAPI::opengl:
		{
			m_p_impl->m_graphics_window = std::make_shared<GLWindow>();
			m_p_impl->m_graphics_window->Create(context_width, context_height, context_title);
			m_p_impl->m_graphics_window->SetClearColor({ 0.3f, 0.4f, 0.5f });

			// register and compile shaders
			libgraphics::ResourceManager::RegisterResource(ResourceParams{ libgraphics::ResourceType::shaders, "default_shader", std::make_shared<GLShader>("../fuzzy-libgraphics/shaders/glsl/vertex.glsl", "../fuzzy-libgraphics/shaders/glsl/fragment.glsl") });
			libgraphics::ResourceManager::RegisterResource(ResourceParams{ libgraphics::ResourceType::shaders, "skybox_shader", std::make_shared<GLShader>("../fuzzy-libgraphics/shaders/glsl/skybox_vert.glsl", "../fuzzy-libgraphics/shaders/glsl/skybox_frag.glsl") });

			m_sky_box = std::make_shared<GLSkybox>();

			m_p_impl->m_main_camera = {};

			const auto directional_light = std::make_shared<Light>();
			directional_light->m_direction = glm::vec3{-1.0f, 1.0f, 0.0f};
			directional_light->m_type = 0;
			directional_light->m_color = glm::vec4(1.0f);
			directional_light->m_is_active = true;

			AddLight(directional_light);

			m_test_cube = std::make_shared<Model>("../resources/astronaut_pose.glb");
		}
		break;
		case GraphicsAPI::directx: break;
		default: break;
		}
	}

	auto Core::Update(const RenderFunction& render_function) -> void
	{
		const auto glfw_window = static_cast<GLFWwindow*>(m_p_impl->m_graphics_window->GetNativeHandle()->GetNativeHandle());

		auto previous_time = glfwGetTime();

		const auto& default_shader = libgraphics::ResourceManager::GetFromCache<GLShader>({ libgraphics::ResourceType::shaders, "default_shader" });
		const auto& skybox_shader = libgraphics::ResourceManager::GetFromCache<GLShader>({ libgraphics::ResourceType::shaders, "skybox_shader" });

		auto test_win = libgraphics::gui::GUIWindowStats{};

		while (!glfwWindowShouldClose(glfw_window))
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::ShowDemoWindow();
			test_win.Render();

			const auto current_time = glfwGetTime();
			m_delta_time = current_time - previous_time;
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
			m_p_impl->m_main_camera.Animate(m_p_impl->m_graphics_window, m_delta_time);

			m_sky_box->Render(skybox_shader.value());
			m_test_cube->Render(default_shader.value());

			if (render_function)
			{
				render_function(m_delta_time);
			}

			ImGui::Render();

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			m_p_impl->m_graphics_window->SwapBuffers();
		}

		m_p_impl->m_graphics_window->Destroy();
	}

	auto Core::GetInstance() -> Core&
	{
		static auto core = Core();
		return core;
	}

	auto Core::AddLight(const std::shared_ptr<Light>& light) -> void
	{
		m_lights.push_back(light);
	}

	auto Core::GetLights() const -> std::vector<std::shared_ptr<Light>>
	{
		return m_lights;
	}
}
