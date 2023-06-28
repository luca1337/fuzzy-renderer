#pragma once

#include <entity/entity.h>

namespace libgraphics
{
	class Component
	{
	public:
		virtual ~Component() = default;
		Component() = default;

		virtual void Start() {}
		virtual void Update(float delta_time) {}
		virtual void Render() {}

		[[nodiscard]] auto& GetEntity() const -> Entity { return entity; }
		auto SetEntity(const Entity& entity) { this->entity = entity; }

	private:
			Entity m_entity = {};
	};
}