#include <entities/entity.h>

namespace libgraphics
{
	auto Entity::Render() const -> void
	{
		for (const auto& component : m_components | std::views::values)
		{
			component->Render();
		}
	}

	auto Entity::Update(const float delta_time) const -> void
	{
		for (const auto& component : m_components | std::views::values)
		{
			component->Update(delta_time);
		}
	}
}
