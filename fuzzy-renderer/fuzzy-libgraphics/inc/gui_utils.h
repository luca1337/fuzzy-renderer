#pragma once

#include <string>

namespace libgraphics::utils::gui
{
	/**
	 * \brief Call this before calling any button, text, gui element: the next element will be centered
	 * \param name Text name, this must be the same as the text you will put on your element (button, raw text.. etc..)
	 */
	auto CenterGuiElement(const std::string& name) -> void;

	auto ShowEngineStatsOverlay() -> void;
}
