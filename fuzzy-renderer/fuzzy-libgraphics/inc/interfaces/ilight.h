#pragma once

#include <enums.h>

namespace libgraphics
{
	class ILight
	{
	public:
		virtual ~ILight() = default;
		LightType m_type = {};
	};
}