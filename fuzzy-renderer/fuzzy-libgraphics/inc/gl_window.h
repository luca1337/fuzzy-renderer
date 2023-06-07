#pragma once

#include "igraphics_window.h"

namespace libgraphics
{
	class IGraphicsContext;

	class LIBGRAPHICS_API GLWindow : public IGraphicsWindow
	{
	public:
		auto Create() -> void override;
		auto Destroy() -> void override;
		[[nodiscard]] auto GetNativeHandle() const -> const std::shared_ptr<IGraphicsContext> & override { return _graphicsContext; }

	private:

		std::shared_ptr<IGraphicsContext> _graphicsContext;
	};
}
