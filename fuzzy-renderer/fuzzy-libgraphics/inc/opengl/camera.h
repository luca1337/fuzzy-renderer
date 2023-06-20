#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <optional>
#include <memory>

namespace libgraphics
{
	class IGraphicsWindow;

	enum class CameraDirection
	{
		front,
		back,
		left,
		right,
		up,
		down
	};

	struct CameraProps
	{
		glm::dvec3 m_world_position = { 0.0f, 0.0f, 5.0f };
		glm::mat4 m_projection = glm::identity<glm::mat4>();
		double m_yaw = { -90.0 };
		double m_pitch = { 0.0 };
		double m_fov = { 65.0 };
	};

	/**
	 * \brief Use this if you want the classic view matrix of the camera.
	 * \return return a 4x4 view matrix with all components TRS.
	 */
	auto GetViewMatrix(const CameraProps&) -> glm::mat4;

	/**
	 * \brief Use this if you want to remove the translation section of transformation matrix by taking the upper-left 3x3 matrix of the 4x4 matrix.
	 * \return return a 3x3 matrix without translation.
	 */
	auto GetViewMatrix3(const CameraProps&) -> glm::mat3;


	auto ComputeCameraProjection(const double fov, const double width, const double height, const double z_near, const double z_far) -> glm::mat4;

	class Camera
	{
	public:
		auto GetWorldPosition() const -> glm::dvec3;
		auto RotateByMouse(const std::shared_ptr<IGraphicsWindow>& window) -> void;
		auto Animate(const std::shared_ptr<IGraphicsWindow>& window, const double delta_time) -> void;
		void Reset() { m_previous_position.reset(); }
		CameraProps m_camera_props = {};
	private:
		auto Translate(const CameraDirection, const double delta_time) -> void;
		std::optional<glm::dvec2> m_previous_position;
	};
}