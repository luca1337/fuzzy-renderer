#include <opengl/camera.h>
#include <opengl/gl_context.h>
#include <opengl/gl_window.h>

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "core.h"

namespace libgraphics
{
	auto CalculateDirectionVectors(const CameraProps& camera_props) -> std::tuple<glm::dvec3, glm::dvec3, glm::dvec3>
	{
		const auto yaw_rad = glm::radians(camera_props.m_yaw);
		const auto pitch_rad = glm::radians(camera_props.m_pitch);

		const glm::dvec3 front(
			cos(pitch_rad) * -sin(yaw_rad),  // Inversione della X
			-sin(pitch_rad),
			cos(pitch_rad) * cos(yaw_rad)   // Inversione della Z
		);

		constexpr glm::dvec3 world_up(0.0, 1.0, 0.0);
		const auto right = glm::normalize(glm::cross(front, world_up));
		const auto up = glm::normalize(glm::cross(right, front));

		return { front, right, up };
	}

	auto GetViewMatrix(const CameraProps& camera_props) -> glm::mat4
	{
		const auto [front, right, up] = CalculateDirectionVectors(camera_props);
		return glm::lookAt(camera_props.m_world_position, camera_props.m_world_position + front, up);
	}

	auto GetViewMatrix3(const CameraProps& camera_props) -> glm::mat3
	{
		const auto [front, right, up] = CalculateDirectionVectors(camera_props);
		return glm::mat3{glm::lookAt(camera_props.m_world_position, camera_props.m_world_position + front, up)};
	}

	auto ComputeCameraProjection(const double fov, const double width, const double height, const double z_near, const double z_far) -> glm::mat4
	{
		return glm::perspective(glm::radians(fov), width / height, z_near, z_far);
	}

	auto Camera::GetWorldPosition() const -> glm::dvec3
	{
		return m_camera_props.m_world_position;
	}

	auto Camera::GetWorldRotation() const -> glm::dvec3
	{
		const auto camera_matrix = glm::inverse(m_camera_props.m_projection) * GetViewMatrix(m_camera_props);

		glm::vec3 translation, scale, skew;
		glm::vec4 perspective;
		glm::quat rotation;

		glm::decompose(camera_matrix, scale, rotation, translation, skew, perspective);

		const auto euler_angles = glm::eulerAngles(rotation);

		return glm::degrees(euler_angles);
	}

	bool first_mouse = true;
	double last_x = {}, last_y = {};
	double x_pos = {}, y_pos = {};
	auto Camera::RotateByMouse(const std::shared_ptr<IGraphicsWindow>& window) -> void
	{
		const auto glfw_native_window_handle = static_cast<GLFWwindow*>(window->GetNativeHandle()->GetNativeHandle());

		glfwGetCursorPos(glfw_native_window_handle, &x_pos, &y_pos);

		const auto delta = (glm::dvec2(x_pos, y_pos) - m_previous_position.value_or(glm::dvec2(x_pos, y_pos))) * 0.1;
		m_previous_position = glm::dvec2(x_pos, y_pos);

		m_camera_props.m_yaw += delta.x;
		m_camera_props.m_pitch += delta.y;

		// make sure that when m_pitch is out of bounds, screen doesn't get flipped.
		constexpr auto max_pitch = 89.9;
		if (m_camera_props.m_pitch >= max_pitch)	m_camera_props.m_pitch = max_pitch;
		if (m_camera_props.m_pitch <= -max_pitch) m_camera_props.m_pitch = -max_pitch;
	}

	auto Camera::Animate(const std::shared_ptr<IGraphicsWindow>& window, const double delta_time) -> void
	{
		const auto glfw_native_window_handle = static_cast<GLFWwindow*>(window->GetNativeHandle()->GetNativeHandle());

		if (glfwGetKey(glfw_native_window_handle, GLFW_KEY_W))
		{
			InternalTranslate(CameraDirection::front, delta_time);
		}
		if (glfwGetKey(glfw_native_window_handle, GLFW_KEY_S))
		{
			InternalTranslate(CameraDirection::back, delta_time);
		}
		if (glfwGetKey(glfw_native_window_handle, GLFW_KEY_A))
		{
			InternalTranslate(CameraDirection::left, delta_time);
		}
		if (glfwGetKey(glfw_native_window_handle, GLFW_KEY_D))
		{
			InternalTranslate(CameraDirection::right, delta_time);
		}
		if (glfwGetKey(glfw_native_window_handle, GLFW_KEY_E))
		{
			InternalTranslate(CameraDirection::up, delta_time);
		}
		if (glfwGetKey(glfw_native_window_handle, GLFW_KEY_Q))
		{
			InternalTranslate(CameraDirection::down, delta_time);
		}
	}

	auto Camera::ScreenPointToRay3D(const glm::vec2& screen_coords) const -> Ray
	{
		const auto& core = libgraphics::Core::GetInstance();
		const auto gl_context = std::static_pointer_cast<libgraphics::GLContext>(core.GetGraphicsWindow()->GetNativeHandle());

		const auto mouse_screen_ndc_coords = glm::vec2{
			2.0f * screen_coords.x / static_cast<float>(gl_context->Data().m_width) - 1.0f,
			2.0f * screen_coords.y / static_cast<float>(gl_context->Data().m_height) - 1.0f,
		};

		const auto& view = GetViewMatrix(core.GetMainCamera().m_camera_props);
		const auto& proj = libgraphics::ComputeCameraProjection(60.0, gl_context->Data().m_width, gl_context->Data().m_height, 0.01, 1000.0);

		const auto inverse_view_projection = glm::inverse(proj * view);
		const auto screen_pos = glm::vec4{ mouse_screen_ndc_coords.x, -mouse_screen_ndc_coords.y, 1.0f, 1.0f };
		const auto world_pos = inverse_view_projection * screen_pos;

		const auto ray_origin = GetWorldPosition();
		const auto ray_direction = glm::normalize(glm::vec3{ world_pos });

		return { ray_origin, ray_direction };
	}

	auto Camera::InternalTranslate(const CameraDirection direction, const double delta_time) -> void
	{
		const auto [front, right, up] = CalculateDirectionVectors(m_camera_props);
		switch (direction)
		{
		case (CameraDirection::front):	m_camera_props.m_world_position += front * 15.0 * delta_time; break;
		case (CameraDirection::back):	m_camera_props.m_world_position -= front * 15.0 * delta_time; break;
		case (CameraDirection::left):	m_camera_props.m_world_position -= right * 15.0 * delta_time; break;
		case (CameraDirection::right):	m_camera_props.m_world_position += right * 15.0 * delta_time; break;
		case (CameraDirection::up):		m_camera_props.m_world_position += up * 15.0 * delta_time; break;
		case (CameraDirection::down):	m_camera_props.m_world_position -= up * 15.0 * delta_time; break;
		default:break;
		}
	}
}
