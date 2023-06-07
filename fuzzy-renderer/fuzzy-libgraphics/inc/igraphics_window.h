#pragma once

#include "framework.h"

namespace libgraphics
{
	class IGraphicsContext;

	/// <summary>
	/// Base Interface for all graphics windows (OpenGL, DirectX, Vulkan..)
	/// </summary>
	class LIBGRAPHICS_API IGraphicsWindow
	{
	public:
		virtual ~IGraphicsWindow() = default;

		virtual auto Create() -> void = 0;
		virtual auto Destroy() -> void = 0;
		[[nodiscard]]  virtual auto GetNativeHandle() const -> const std::shared_ptr<IGraphicsContext>& = 0;
	};
}