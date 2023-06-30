#include <gui/windows/gui_window_left_panel.h>

#include <gui_utils.h>
#include <logger.h>

namespace libgraphics::gui
{
	GUIWindowLeftPanel::GUIWindowLeftPanel()
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


	void GUIWindowLeftPanel::Render()
	{
		utils::gui::RenderWindowContent(m_title, m_is_open, m_size, m_position, m_flags, m_bg_alpha, [&] {

			if (ImGui::BeginTabBar("TabBar"))
			{
				if (ImGui::BeginTabItem("Tab 1"))
				{
					ImGui::MenuItem("pippo");
					ImGui::MenuItem("pluto");

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Tab 2"))
				{
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Tab 3"))
				{
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		});
	}
}