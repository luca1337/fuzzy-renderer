#pragma once

#include "interfaces/igraphics_window.h"

namespace libgraphics
{
	class IGraphicsContext;

	class LIBGRAPHICS_API GLWindow : public IGraphicsWindow
	{
	public:
		auto Create(const int width, const int height, const std::string_view title) -> void override;
		auto Destroy() -> void override;
		auto Clear() -> void override;
		auto SwapBuffers() -> void override;
		[[nodiscard]] auto GetNativeHandle() const -> const std::shared_ptr<IGraphicsContext> & override { return _graphicsContext; }

	private:

		std::shared_ptr<IGraphicsContext> _graphicsContext;
	};
}
