#pragma once

#include <entities/entity.h>

namespace libgraphics
{
	class Model : public Entity
	{
	public:
		explicit Model(const std::string_view path);
	};
}
