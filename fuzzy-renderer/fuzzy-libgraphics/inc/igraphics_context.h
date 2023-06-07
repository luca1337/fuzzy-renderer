#pragma once

#include "framework.h"

namespace libgraphics
{
	struct LIBGRAPHICS_API ContextData
	{
		int Width = {};
		int Height = {};
		std::string_view Title = {};
		void* NativeHandle = {};
	};

	class LIBGRAPHICS_API IGraphicsContext
	{
	public:
		virtual ~IGraphicsContext() = default;

		virtual auto Init(const int width, const int height, const std::string_view title) -> void = 0;
		virtual auto Shutdown() -> void = 0;
	};

}
