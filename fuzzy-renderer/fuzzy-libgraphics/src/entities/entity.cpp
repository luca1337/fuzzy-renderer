#include <entities/entity.h>
#include <ranges>

namespace libgraphics
{
	auto Entity::Render() -> void
	{
		for (const auto& component : std::views::values(m_components))
		{
			component->Render();
		}
	}

	auto Entity::Update(const float delta_time) -> void
	{
		for (const auto& component : std::views::values(m_components))
		{
			component->Update(delta_time);
		}
	}
}
