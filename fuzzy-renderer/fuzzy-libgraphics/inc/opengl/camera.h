#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <optional>
#include <memory>

#include <ray.h>

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
		glm::dvec3 m_world_position = { 0.0f, 0.0f, -5.0f };
		glm::mat4 m_projection = glm::identity<glm::mat4>();
		double m_yaw = { 0.0 };
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
		/**
		 * Retrieves the world position of an object.
		 * @return The world position as a 3D vector.
		 */
		[[nodiscard]] auto GetWorldPosition() const->glm::dvec3;

		/**
		 * Retrieves the world rotation of an object.
		 * @return The world rotation as a 3D vector.
		 */
		[[nodiscard]] auto GetWorldRotation() const->glm::dvec3;

		/**
		 * Rotates an object based on mouse input.
		 * @param window A shared pointer to the graphics window for capturing mouse input.
		 */
		auto RotateByMouse(const std::shared_ptr<IGraphicsWindow>& window) -> void;

		/**
		 * Performs animation updates for an object.
		 * @param window A shared pointer to the graphics window.
		 * @param delta_time The elapsed time since the last frame.
		 */
		auto Animate(const std::shared_ptr<IGraphicsWindow>& window, const double delta_time) -> void;

		/**
		 * \brief Resets the camera's states.
		 */
		auto Reset() -> void { m_previous_position.reset(); }

		/**
		 * Computes a 3D ray from screen coordinates to the world space.
		 * @param screen_coords The screen coordinates of the point.
		 * @return The computed 3D ray in world space.
		 */
		[[nodiscard]] auto ScreenPointToRay3D(const glm::vec2& screen_coords) const->Ray;

		/**
		 * \brief Access camera data (yaw, pitch, position...)
		 */
		CameraProps m_camera_props = {};
	private:
		auto InternalTranslate(const CameraDirection, const double delta_time) -> void;
		std::optional<glm::dvec2> m_previous_position = {};
	};
}
