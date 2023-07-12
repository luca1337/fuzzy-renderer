#include <entities/game_camera.h>
#include <components/camera.h>

namespace libgraphics
{
	GameCamera::GameCamera()
	{
		AddComponent(std::make_shared<Camera>());
	}

	auto GameCamera::Update(float delta_time) -> void
	{
		Entity::Update(delta_time);
	}
}
