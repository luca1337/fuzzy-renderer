#include <core.h>
#include <format>
#include <gui_utils.h>
#include <opengl/gl_context.h>

#include <imgui.h>

namespace libgraphics::utils::gui
{
    auto IsMouseInsideWindow()
    {
        const auto& mouse_position = ImGui::GetMousePos();
        const auto& core = Core::GetInstance();
        const auto& gl_context = ::std::static_pointer_cast<GLContext>(core.GetGraphicsWindow()->GetNativeHandle());

        return (mouse_position.x * (static_cast<float>(1) / gl_context->Data().m_width) >= 0 && mouse_position.x <= gl_context->Data().m_width) && (
	        mouse_position.y / gl_context->Data().m_height >= 0 && mouse_position.y <= gl_context->Data().m_height);
    }

    auto CenterGuiElement(const std::string& name) -> void
    {
        const auto window_width = ImGui::GetWindowWidth();
        const auto text_width = ImGui::CalcTextSize(name.c_str()).x;
        const auto center_x = (window_width - text_width) * 0.5f;
        ImGui::SetCursorPosX(center_x);
    }

    auto ShowEngineStatsOverlay() -> void
    {
        const auto& io = ImGui::GetIO();
        constexpr auto window_flags =
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoFocusOnAppearing |
            ImGuiWindowFlags_NoNav |
            ImGuiWindowFlags_NoMove;

        auto is_open = true;

        ImGui::SetNextWindowBgAlpha(0.5f); // Transparent background
        ImGui::SetNextWindowSize({230.0f, 0.0f});
        ImGui::SetNextWindowPos({1.0f, 1.0f});

        if (const auto& stats_text = "- Engine Stats -"; ImGui::Begin(stats_text, &is_open, window_flags))
        {
            CenterGuiElement(stats_text);
            ImGui::Text(stats_text);
            ImGui::Separator();

            // Frames (Frame rate && Frame time)
            ImGui::Text("FPS: %.1f/s", io.Framerate);
            ImGui::Text("Frame Time: %.3f ms", 1000.0f / io.Framerate);

            ImGui::PushStyleColor(ImGuiCol_Separator, { 1.0f, 0.0f, 0.0f, 1.0f });
            ImGui::Separator();
            ImGui::PopStyleColor();

            // Mouse position
            const auto& mouse_position = ImGui::GetMousePos();
            ImGui::Text("Input/Output:");

            const auto& mouse_position_text = IsMouseInsideWindow() ? std::format("Mouse: [X: {:.1f} | Y: {:.1f}]", mouse_position.x, mouse_position.y) : "Mouse is out of window!";
            ImGui::Text(mouse_position_text.c_str());

            // Objects in the world

        }

        ImGui::End();
    }



}
