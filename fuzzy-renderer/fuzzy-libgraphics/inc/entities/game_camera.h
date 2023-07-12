#pragma once

#include <entities/entity.h>

#include <components/camera.h>

namespace libgraphics
{
	class GameCamera : public Entity
	{
	public:
		GameCamera();
		auto Update(float delta_time) -> void override;
		[[nodiscard]] auto GetNativeCamera() const { return GetComponent<Camera>(); }
	};
}
