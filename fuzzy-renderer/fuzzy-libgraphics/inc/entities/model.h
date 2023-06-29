#pragma once

#include <memory>
#include <vector>

#include <entities/entity.h>

namespace libgraphics
{
	class Model : Entity
	{
	public:
		explicit Model(const std::string_view path);
		auto Update() const -> void;

	private:
		std::vector<std::shared_ptr<Entity>> m_entities = {};
	};
}
