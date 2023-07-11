#include <core.h>
#include <enums.h>
#include <filesystem>

#include <logger.h>
#include <resource_manager.h>
#include <entities/model.h>
#include <opengl/gl_context.h>
#include <opengl/gl_shader.h>
#include <opengl/gl_skybox.h>
#include <opengl/gl_window.h>

#include <gui_utils.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

#include <gui/windows/gui_window_left_panel.h>
#include <gui/windows/gui_window_stats.h>
#include <gui/windows/gui_menu_bar.h>
#include <rendering/light.h>

#include <entity_manager.h>

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
			// Create window
			m_p_impl->m_graphics_window = std::make_shared<GLWindow>();
			m_p_impl->m_graphics_window->Create(context_width, context_height, context_title);
			m_p_impl->m_graphics_window->SetClearColor({ 0.3f, 0.4f, 0.5f });

			// Register default resources shaders
			libgraphics::ResourceManager::RegisterResource(ResourceParams{ libgraphics::ResourceType::shaders, "default_shader", std::make_shared<GLShader>("../fuzzy-libgraphics/shaders/glsl/vertex.glsl", "../fuzzy-libgraphics/shaders/glsl/fragment.glsl") });
			libgraphics::ResourceManager::RegisterResource(ResourceParams{ libgraphics::ResourceType::shaders, "skybox_shader", std::make_shared<GLShader>("../fuzzy-libgraphics/shaders/glsl/skybox_vert.glsl", "../fuzzy-libgraphics/shaders/glsl/skybox_frag.glsl") });

			const auto& default_shader = libgraphics::ResourceManager::GetFromCache<GLShader>({ libgraphics::ResourceType::shaders, "default_shader" });

			default_shader.value()->AllocateLightsBuffer("LightsBlock");

			const auto directional_light = std::make_shared<Light>();
			directional_light->m_direction = glm::vec3{ 0.7f, 0.7f, 0.0 };
			directional_light->m_type = 0;
			directional_light->m_intensity = 1.0f;
			directional_light->m_color = glm::vec4(1.0f);
			directional_light->m_is_active = true;

			AddLight(directional_light);

			m_entity_manager = std::make_shared<EntityManager>();

			m_sky_box = std::make_shared<GLSkybox>();

			m_p_impl->m_main_camera = {};

			m_entity_model = std::make_shared<Model>("../resources/Cube.glb");
			m_entity_model->SetName("Cube");
			m_entity_manager->AddEntity(m_entity_model);

			/*m_entity_model2 = std::make_shared<Model>("../resources/rock_fountain.glb");
			m_entity_model2->SetName("rock_fountain");
			m_entity_manager->AddEntity(m_entity_model2);*/
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

		const auto& skybox_shader = libgraphics::ResourceManager::GetFromCache<GLShader>({ libgraphics::ResourceType::shaders, "skybox_shader" });

		auto gui_menu_bar = libgraphics::gui::GUIMenuBar{};
		auto test_win = libgraphics::gui::GUIWindowStats{};
		auto gui_lp = libgraphics::gui::GUIWindowLeftPanel{};

		while (!glfwWindowShouldClose(glfw_window))
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::ShowDemoWindow();
			gui_menu_bar.Render();
			gui_lp.Render();

			const auto current_time = glfwGetTime();
			m_delta_time = static_cast<float>(current_time - previous_time);
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

			m_entity_manager->Render();
			m_entity_manager->Update(m_delta_time);

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
