#pragma once

#include <imgui.h>
#include <string>

namespace libgraphics::gui
{
	class GUIObject
	{
	public:
		virtual ~GUIObject() = default;
		virtual auto Render() -> void = 0;
	};

	class GUIWindow : public GUIObject
	{
	public:
		GUIWindow() = default;

		~GUIWindow() override = default;

		auto SetPosition(const ImVec2& position) -> void { m_position = position; }
		auto GetPosition() const -> ImVec2 { return m_position; }

		auto SetSize(const ImVec2& size) -> void { m_size = size; }
		auto GetSize() const -> ImVec2 { return m_size; }

	protected:
		ImVec2 m_size = {};
		ImVec2 m_position = {};

		std::string m_title = {};
		ImGuiWindowFlags m_flags = {};
		float m_bg_alpha = 1.0f; // fully opaque
	};

	class GUIStats : public GUIWindow
	{
	public:
		GUIStats() : GUIWindow()
		{
			m_title = "Test Window";
			m_bg_alpha = 0.75f;
			m_size = { 230.0f, 130.0f };
			m_position = { 1.0f, 1.0f };
			m_flags = ImGuiWindowFlags_NoFocusOnAppearing;
		}

		void Render() override
		{
			auto is_open = true;

			ImGui::SetNextWindowBgAlpha(m_bg_alpha);
			ImGui::SetNextWindowSize({ m_size.x, m_size.y });
			ImGui::SetNextWindowPos({ m_position.x, m_position.y });

			if (ImGui::Begin(m_title.c_str(), &is_open, m_flags)) 
			{
				// do something
			}

			ImGui::End();
		}
	};


}
