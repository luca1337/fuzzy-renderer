#include <opengl/camera.h>
#include <opengl/gl_window.h>
#include <opengl/gl_context.h>

#include <GLFW/glfw3.h>


#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace libgraphics
{
	auto CalculateDirectionVectors(const CameraProps& camera_props) -> std::tuple<glm::dvec3, glm::dvec3, glm::dvec3>
	{
		glm::dvec3 direction = {};
		direction.x = cos(glm::radians(camera_props.m_yaw)) * cos(glm::radians(camera_props.m_pitch));
		direction.y = -sin(glm::radians(camera_props.m_pitch));
		direction.z = sin(glm::radians(camera_props.m_yaw)) * cos(glm::radians(camera_props.m_pitch));
		const auto front = glm::normalize(direction);

		constexpr glm::dvec3 world_up = { 0.0, 1.0, 0.0 };

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
			Translate(CameraDirection::front, delta_time);
		}
		if (glfwGetKey(glfw_native_window_handle, GLFW_KEY_S))
		{
			Translate(CameraDirection::back, delta_time);
		}
		if (glfwGetKey(glfw_native_window_handle, GLFW_KEY_A))
		{
			Translate(CameraDirection::left, delta_time);
		}
		if (glfwGetKey(glfw_native_window_handle, GLFW_KEY_D))
		{
			Translate(CameraDirection::right, delta_time);
		}
		if (glfwGetKey(glfw_native_window_handle, GLFW_KEY_E))
		{
			Translate(CameraDirection::up, delta_time);
		}
		if (glfwGetKey(glfw_native_window_handle, GLFW_KEY_Q))
		{
			Translate(CameraDirection::down, delta_time);
		}
	}

	auto Camera::Translate(const CameraDirection direction, const double delta_time) -> void
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