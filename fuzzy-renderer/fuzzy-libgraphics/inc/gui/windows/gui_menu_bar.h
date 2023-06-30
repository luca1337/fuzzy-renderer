#pragma once

#include <gui/base/gui_object_base.h>

namespace libgraphics::gui
{
	class GUIMenuBar : public GUIObjectBase
	{
	public:
		auto Render() -> void override;
	};
}
