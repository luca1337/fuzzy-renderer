#include <core.h>
#include <gui_utils.h>
#include <logger.h>
#include <resource_manager.h>
#include <gui/windows/gui_window_stats.h>
#include <opengl/gl_shader.h>

namespace libgraphics::gui
{
	GUIWindowStats::GUIWindowStats()
	{
		m_title = "Stats Window";
		m_bg_alpha = 0.75f;
		m_size = { 300.0f, 0.0f };
		m_position = { 1.0f, 1.0f };
		m_flags = ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoFocusOnAppearing |
			ImGuiWindowFlags_NoNav |
			ImGuiWindowFlags_NoMove;
	}

	void GUIWindowStats::Render()
	{
		utils::gui::RenderWindowContent(m_title, m_is_open, m_size, m_position, m_flags, m_bg_alpha, [&] {

			// running this template without any additional piece of code will open a default window
			const auto& io = ImGui::GetIO();

			utils::gui::CenterGuiElement(m_title);
			ImGui::Text(m_title.c_str());
			ImGui::Separator();
			ImGui::Spacing();

			// Frames (Frame rate && Frame time)
			ImGui::Text("FPS: %.1f", io.Framerate);
			ImGui::Text("Frame Time: %.3f ms", 1000.0f / io.Framerate);

			utils::gui::Separator(utils::gui::ColorRed);

			// Mouse position
			const auto& mouse_position = ImGui::GetMousePos();
			ImGui::Text("Input/Output:");
			ImGui::Spacing();

			const auto& mouse_position_text = utils::gui::IsMouseInsideWindow() ? std::format("Mouse: [X: {:.1f} | Y: {:.1f}]", mouse_position.x, mouse_position.y) : "Mouse is out of window!";
			ImGui::Text(mouse_position_text.c_str());

			utils::gui::Separator(utils::gui::ColorRed);
			ImGui::Text("Engine Cache:");
			ImGui::Spacing();

			for (auto resource_type_idx = 0; resource_type_idx != static_cast<int>(ResourceType::max_enum); resource_type_idx++)
			{
				const auto& resource_enum_type_name = ResourceTypeToString(static_cast<ResourceType>(resource_type_idx));

				if (ImGui::TreeNode(resource_enum_type_name.c_str()))
				{
					const auto& resources = ResourceManager::GetAllFromCache<libgraphics::GLShader>();
					const auto& shaders_count = std::views::iota(0) | std::views::take(resources.size());

					std::ranges::for_each(shaders_count, [&](const auto shader_idx)
					{
						if (resources[shader_idx].m_resource_type == static_cast<ResourceType>(resource_type_idx))
						{
							ImGui::MenuItem(resources[shader_idx].m_name.c_str());
						}
					});

					ImGui::TreePop();
				}
			}

			utils::gui::Separator(utils::gui::ColorRed);
			ImGui::Text("Camera:");
			ImGui::Spacing();
		});
	}

}
