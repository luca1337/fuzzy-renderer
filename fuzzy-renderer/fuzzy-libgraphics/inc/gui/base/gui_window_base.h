#pragma once

#include <gui/base/gui_object_base.h>

#include <imgui.h>
#include <string>

namespace libgraphics::gui
{
	class GUIWindowBase : public GUIObjectBase
	{
	public:
		GUIWindowBase() = default;
		~GUIWindowBase() override = default;

		auto Render() -> void override;

	protected:
		auto SetPosition(const ImVec2& position) -> void { m_position = position; }
		auto GetPosition() const -> ImVec2 { return m_position; }

		auto SetSize(const ImVec2& size) -> void { m_size = size; }
		auto GetSize() const -> ImVec2 { return m_size; }

		bool m_is_open = {};
		ImVec2 m_size = {};
		ImVec2 m_position = {};

		std::string m_title = {};
		ImGuiWindowFlags m_flags = {};
		float m_bg_alpha = 1.0f; // fully opaque
	};
}
