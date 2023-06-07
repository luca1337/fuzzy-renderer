#pragma once

#include "igraphics_window.h"

class IGraphicsContext;

namespace libgraphics
{
	class LIBGRAPHICS_API GLWindow : public IGraphicsWindow
	{
	public:
		auto Create() -> void override;
		auto Destroy() -> void override;
		[[nodiscard]] auto GetNativeHandle() const -> void* override;

	private:

		std::shared_ptr<IGraphicsContext> _graphicsContext;
	};
}
