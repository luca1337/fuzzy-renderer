#pragma once

#include <gui/base/gui_window_base.h>

namespace libgraphics::gui
{
	class GUIWindowLeftPanel : public GUIWindowBase
	{
	public:
		GUIWindowLeftPanel();

		void Render() override;
	};


}