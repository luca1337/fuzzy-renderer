#pragma once

#include <memory>
#include <optional>
#include <ray.h>
#include <components/component.h>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
		glm::mat4 m_projection = glm::identity<glm::mat4>();
		double m_yaw = { 0.0 };
		double m_pitch = { 0.0 };
		double m_fov = { 60.0 };
		double m_z_near = { 0.01 };
		double m_z_far = { 1000.0 };
	};

	auto ComputeCameraProjection(double fov, double width, double height, double z_near, double z_far) -> glm::mat4;

	class Camera : public Component
	{
	public:
		/**
		 * \brief Rotates an object based on mouse input.
		 * \param window A shared pointer to the graphics window for capturing mouse input.
		 */
		auto RotateByMouse(const std::shared_ptr<IGraphicsWindow>& window) -> void;

		/**
		 * \brief Performs animation updates for an object.
		 * \param window A shared pointer to the graphics window.
		 * \param delta_time The elapsed time since the last frame.
		 */
		auto Animate(const std::shared_ptr<IGraphicsWindow>& window, float delta_time) const -> void;

		/**
		 * \brief Resets the camera's states.
		 */
		auto Reset() -> void { m_previous_position.reset(); }

		/**
		 * \brief Computes a 3D ray from screen coordinates to the world space.
		 * \param screen_coords The screen coordinates of the point.
		 * \return The computed 3D ray in world space.
		 */
		[[nodiscard]] auto ScreenPointToRay3D(const glm::vec2& screen_coords) const -> Ray;

		[[nodiscard]] auto CalculateDirectionVectors() const -> std::tuple<glm::vec3, glm::vec3, glm::vec3>;

		[[nodiscard]] auto GetViewMatrix() const -> glm::mat4;
		[[nodiscard]] auto GetViewMatrix3() const ->glm::mat3;

		auto& Props() { return m_camera_props; }
		[[nodiscard]] auto Props() const { return m_camera_props; }

	private:
		auto InternalTranslate(CameraDirection, float delta_time) const -> void;

	public:
		auto Initialize() -> void override;
		auto Update(float delta_time) -> void override;

	private:
		std::optional<glm::dvec2> m_previous_position = {};
		std::shared_ptr<Transform> m_transform_component = {};

		CameraProps m_camera_props = {};
	};
}
