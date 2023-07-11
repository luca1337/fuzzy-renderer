#include <core.h>
#include <format>
#include <gui_utils.h>
#include <opengl/gl_context.h>

#include <imgui.h>

namespace libgraphics::utils::gui
{
    auto IsMouseInsideWindow() -> bool
    {
        const auto& mouse_position = ImGui::GetMousePos();
        const auto& core = Core::GetInstance();
        const auto& gl_context = ::std::static_pointer_cast<GLContext>(core.GetGraphicsWindow()->GetNativeHandle());

        return (mouse_position.x * (static_cast<float>(1) / gl_context->Data().m_width) >= 0 && mouse_position.x <= gl_context->Data().m_width) && (
	        mouse_position.y / gl_context->Data().m_height >= 0 && mouse_position.y <= gl_context->Data().m_height);
    }

    auto InterpolateColor(const ImVec4& color1, const ImVec4& color2, const float t) -> ImVec4
    {
        return {
            color1.x + (color2.x - color1.x) * t,
            color1.y + (color2.y - color1.y) * t,
            color1.z + (color2.z - color1.z) * t,
            color1.w + (color2.w - color1.w) * t
        };
    }

    auto RenderWindowContent(const std::string& title, bool& is_open, const ImVec2& size, const ImVec2& position,
	    const ImGuiWindowFlags flags, const float bg_alpha, const std::function<void()>& render_content) -> void
    {
        ImGui::SetNextWindowBgAlpha(bg_alpha);
        ImGui::SetNextWindowSize(size);
        ImGui::SetNextWindowPos(position);

        if (ImGui::Begin(title.c_str(), &is_open, flags)) {
            render_content();
        }

        ImGui::End();
    }

    auto CenterGuiElement(const std::string& name) -> void
    {
        const auto window_width = ImGui::GetWindowWidth();
        const auto text_width = ImGui::CalcTextSize(name.c_str()).x;
        const auto center_x = (window_width - text_width) * 0.5f;
        ImGui::SetCursorPosX(center_x);
    }

    auto Separator(const ImVec4& color) -> void
    {
        ImGui::PushStyleColor(ImGuiCol_Separator, color);
        ImGui::Separator();
        ImGui::PopStyleColor();
    }

    auto ColoredText(const std::string& text, const ImVec4& color) -> void
    {
        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::Text(text.data());
        ImGui::PopStyleColor();
    }
}
