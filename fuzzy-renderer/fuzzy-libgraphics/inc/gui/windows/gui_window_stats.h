#pragma once

#include <gui/base/gui_window_base.h>

namespace libgraphics::gui
{
	class GUIWindowStats : public GUIWindowBase
	{
	public:
		GUIWindowStats();

		void Render() override;
	};


}