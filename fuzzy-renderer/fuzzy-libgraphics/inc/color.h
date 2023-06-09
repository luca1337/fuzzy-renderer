#pragma once

namespace libgraphics
{
	struct Color
	{
		Color(const float r, const float g, const float b, const float a = 1)
			: r(r), g(g), b(b), a(a) {}

		Color() = default;

		float r = {};
		float g = {};
		float b = {};
		float a = {};
	};
}