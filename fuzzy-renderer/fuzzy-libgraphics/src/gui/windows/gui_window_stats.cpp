#include <gui/windows/gui_window_stats.h>

#include <gui_utils.h>

#include <logger.h>

#include <resource_manager.h>

#include "opengl/gl_shader.h"

namespace libgraphics::gui
{
	GUIWindowStats::GUIWindowStats()
	{
		m_title = "Stats Window";
		m_bg_alpha = 0.75f;
		m_size = { 230.0f, 130.0f };
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

			// Frames (Frame rate && Frame time)
			ImGui::Text("FPS: %.1f", io.Framerate);
			ImGui::Text("Frame Time: %.3f ms", 1000.0f / io.Framerate);

			utils::gui::Separator(utils::gui::ColorRed);

			// Mouse position
			const auto& mouse_position = ImGui::GetMousePos();
			ImGui::Text("Input/Output:");

			const auto& mouse_position_text = utils::gui::IsMouseInsideWindow() ? std::format("Mouse: [X: {:.1f} | Y: {:.1f}]", mouse_position.x, mouse_position.y) : "Mouse is out of window!";
			ImGui::Text(mouse_position_text.c_str());

			utils::gui::Separator(utils::gui::ColorRed);
			ImGui::Text("System Cache:");

			auto resources = libgraphics::resources::ResourceManager::GetAllResources<GLShader*>();
			ImGui::Text("Cache size: %i", resources.size());
		});
	}

}
