#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <tuple>
#include <optional>
#include <memory>

namespace libgraphics
{
	class GLWindow;

	enum class CameraDirection
	{
		Front,
		Back,
		Left,
		Right,
		Up,
		Down
	};

	struct CameraProps
	{
		glm::dvec3 worldPosition = { 0.0f, 0.0f, 5.0f };
		glm::mat4 projection = glm::identity<glm::mat4>();
		double yaw = { -90.0 };
		double pitch = { 0.0 };
		double fov = { 65.0 };
	};

	auto GetViewMatrix(const CameraProps&) -> glm::mat4;
	auto ComputeCameraProjection(const double fov, const double width, const double height, const double zNear, const double zFar) -> glm::mat4;

	class Camera
	{
	public:
		auto GetWorldPosition() const->glm::dvec3;
		auto RotateByMouse(std::shared_ptr<GLWindow>& window) -> void;
		auto Animate(std::shared_ptr<GLWindow>& window, const double deltaTime) -> void;
		void Reset() { _previousPosition.reset(); }
		CameraProps CameraProps = {};
	private:
		auto Translate(const CameraDirection, const double deltaTime) -> void;
		std::optional<glm::dvec2> _previousPosition;
	};
}