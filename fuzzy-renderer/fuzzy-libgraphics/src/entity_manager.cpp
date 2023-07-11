#include <entity_manager.h>
#include <entities/entity.h>

namespace libgraphics
{
	auto EntityManager::AddEntity(const std::shared_ptr<Entity>& entity) -> void
	{
		m_entities.push_back(entity);
	}

	auto EntityManager::GetEntityByName(const std::string_view name) const -> std::shared_ptr<Entity>
	{
		for (const auto& entity : m_entities)
		{
			if (entity->GetName() == name)
			{
				return entity;
			}
		}
		return {};
	}

	auto EntityManager::Render() const -> void
	{
		for (const auto& entity : m_entities)
		{
			entity->Render();
		}
	}

	auto EntityManager::Update(const float delta_time) const -> void
	{
		for (const auto& entity : m_entities)
		{
			entity->Update(delta_time);
		}
	}
}
