#pragma once

#include <memory>
#include <vector>

#include <entities/entity.h>

namespace libgraphics
{
	class Model : public Entity
	{
	public:
		explicit Model(const std::string_view path);
		auto Render() -> void override;
		auto Update(float delta_time) -> void override;

	private:
		std::vector<std::shared_ptr<Entity>> m_entities = {};
	};
}
