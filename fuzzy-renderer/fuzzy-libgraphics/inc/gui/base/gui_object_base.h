#pragma once

namespace libgraphics::gui
{
	class GUIObjectBase
	{
	public:
		virtual ~GUIObjectBase() = default;
		virtual auto Render() -> void = 0;
	};
}
