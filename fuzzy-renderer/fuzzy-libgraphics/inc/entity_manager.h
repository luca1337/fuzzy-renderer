#pragma once

#include <memory>
#include <string_view>
#include <vector>

namespace libgraphics
{
	class Entity;

	class EntityManager
	{
	public:
		EntityManager() = default;

		auto AddEntity(const std::shared_ptr<Entity>& entity) -> void;
		[[nodiscard]] auto GetEntityByName(std::string_view name) const->std::shared_ptr<Entity>;
		[[nodiscard]] auto& GetEntities() const { return m_entities; }

		template <typename EntityType>
		auto GetEntityByType() const -> std::shared_ptr<EntityType>
		{
			for (const auto& entity : m_entities)
			{
				if (const auto& derived_entity = std::static_pointer_cast<EntityType>(entity))
				{
					return derived_entity;
				}
			}
			return {};
		}

		auto Render() const -> void;
		auto Update(float delta_time) const -> void;

	private:
		std::vector<std::shared_ptr<Entity>> m_entities = {};
	};
}
