#pragma once

#include <framework.h>

namespace libgraphics
{
	class IGraphicsContext;
	struct Color;

	class LIBGRAPHICS_API IGraphicsWindow
	{
	public:
		virtual ~IGraphicsWindow() = default;

		virtual auto Create(const int width, const int height, const std::string_view title) -> void = 0;
		virtual auto SetClearColor(const Color&) -> void = 0;
		virtual auto Destroy() -> void = 0; 
		virtual auto Clear() -> void = 0;
		virtual auto SwapBuffers() -> void = 0;
		[[nodiscard]]  virtual auto GetNativeHandle() const -> const std::shared_ptr<IGraphicsContext>& = 0;
	};
}