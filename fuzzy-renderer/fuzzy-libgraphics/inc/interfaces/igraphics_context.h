#pragma once

#include "framework.h"

namespace libgraphics
{
	struct LIBGRAPHICS_API ContextData
	{
		int width = {};
		int height = {};
	};

	class LIBGRAPHICS_API IGraphicsContext
	{
	public:
		virtual ~IGraphicsContext() = default;

		virtual auto Init(const int width, const int height, const std::string_view title) -> void = 0;
		virtual auto Shutdown() -> void = 0;
		virtual auto Data() const -> const ContextData& = 0;
		virtual auto Data() -> ContextData& = 0;
		[[nodiscard]] virtual auto GetNativeHandle() const -> void* = 0;
	};

}
