#pragma once

#include <string>
#include <functional>
#include <imgui.h>

namespace libgraphics::utils::gui
{
#pragma region COLORS

	// defaults
	constexpr auto ColorRed = ImVec4{ 1.0f, 0.0f, 0.0f, 1.0f };
	constexpr auto ColorGreen = ImVec4{ 0.0f, 1.0f, 0.0f, 1.0f };
	constexpr auto ColorBlue = ImVec4{ 0.0f, 0.0f, 1.0f, 1.0f };
	constexpr auto ColorYellow = ImVec4{ 1.0f, 1.0f, 0.0f, 1.0f };
	constexpr auto ColorOrange = ImVec4{ 1.0f, 0.5f, 0.0f, 1.0f };
	constexpr auto ColorPurple = ImVec4{ 0.5f, 0.0f, 0.5f, 1.0f };
	constexpr auto ColorPink = ImVec4{ 1.0f, 0.5f, 0.5f, 1.0f };
	constexpr auto ColorCyan = ImVec4{ 0.0f, 1.0f, 1.0f, 1.0f };
	constexpr auto ColorMagenta = ImVec4{ 1.0f, 0.0f, 1.0f, 1.0f };
	constexpr auto ColorBrown = ImVec4{ 0.6f, 0.4f, 0.2f, 1.0f };

	// Mixed
	constexpr auto ColorTeal = ImVec4{ 0.0f, 0.5f, 0.5f, 1.0f };
	constexpr auto ColorLime = ImVec4{ 0.5f, 1.0f, 0.0f, 1.0f };
	constexpr auto ColorOlive = ImVec4{ 0.5f, 0.5f, 0.0f, 1.0f };
	constexpr auto ColorSlateGray = ImVec4{ 0.439f, 0.502f, 0.565f, 1.0f };
	constexpr auto ColorIndigo = ImVec4{ 0.294f, 0.0f, 0.51f, 1.0f };

	auto InterpolateColor(const ImVec4& color1, const ImVec4& color2, const float t) -> ImVec4;

#pragma endregion

	/**
	 * \brief Is the mouse inside the frame-buffer?
	 * \return true or false
	 */
	auto IsMouseInsideWindow() -> bool;

	/**
	 * \brief Use this templated function to generate a default window.
	 * \param title window title
	 * \param is_open whether the window is open or not
	 * \param size size in pixel of window (width, height)
	 * \param position position of imgui window in the frame-buffer
	 * \param flags windows flags to determine the behavior
	 * \param bg_alpha background alpha of window
	 * \param render_content render function to draw content in-between the window
	 */
	auto RenderWindowContent(const std::string& title, bool& is_open, const ImVec2& size, const ImVec2& position, ImGuiWindowFlags flags, float bg_alpha, const std::function<void()>& render_content) -> void;

	/**
	 * \brief Call this before calling any button, text, gui element: the next element will be centered (use it as a decorator)
	 * \param name Text name, this must be the same as the text you will put on your element (button, raw text.. etc..)
	 */
	auto CenterGuiElement(const std::string& name) -> void;

	/**
	 * \brief Adds a separator between elements.
	 * \param color color of the separator
	 */
	auto Separator(const ImVec4& color) -> void;

	auto ColoredText(const std::string& text, const ImVec4& color) -> void;

}
