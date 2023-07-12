#include <opengl/gl_context.h>

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "core.h"

namespace libgraphics
{
	auto Camera::CalculateDirectionVectors() const -> std::tuple<glm::vec3, glm::vec3, glm::vec3>
	{
		const auto yaw_rad = glm::radians(m_camera_props.m_yaw);
		const auto pitch_rad = glm::radians(m_camera_props.m_pitch);

		const glm::vec3 front(
			cos(pitch_rad) * -sin(yaw_rad),  // Inversione della X
			-sin(pitch_rad),
			cos(pitch_rad) * cos(yaw_rad)   // Inversione della Z
		);

		constexpr glm::vec3 world_up(0.0, 1.0, 0.0);
		const auto right = glm::normalize(glm::cross(front, world_up));
		const auto up = glm::normalize(glm::cross(right, front));

		return { front, right, up };
	}

	auto Camera::GetViewMatrix() const -> glm::mat4
	{
		const auto [front, right, up] = CalculateDirectionVectors();
		return glm::lookAt(m_transform_component->GetLocalTranslation(), m_transform_component->GetLocalTranslation() + front, up);
	}

	auto Camera::GetViewMatrix3() const -> glm::mat3
	{
		const auto [front, right, up] = CalculateDirectionVectors();
		return glm::mat3{glm::lookAt(m_transform_component->GetLocalTranslation(), m_transform_component->GetLocalTranslation() + front, up)};
	}

	auto ComputeCameraProjection(const double fov, const double width, const double height, const double z_near, const double z_far) -> glm::mat4
	{
		return glm::perspective(glm::radians(fov), width / height, z_near, z_far);
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
		if (m_camera_props.m_pitch >= max_pitch) m_camera_props.m_pitch = max_pitch;
		if (m_camera_props.m_pitch <= -max_pitch) m_camera_props.m_pitch = -max_pitch;
	}

	auto Camera::Animate(const std::shared_ptr<IGraphicsWindow>& window, const float delta_time) const -> void
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

		// porto le coordinate del mouse nello schermo comprese tra [-1, 1]
		const auto mouse_screen_ndc_coords = glm::vec2{
			2.0f * screen_coords.x / static_cast<float>(gl_context->Data().m_width) - 1.0f,
			2.0f * screen_coords.y / static_cast<float>(gl_context->Data().m_height) - 1.0f,
		};

		const auto& native_camera = core.GetMainCamera()->GetNativeCamera();
		const auto& camera_props = native_camera->m_camera_props;

		const auto& view = GetViewMatrix();
		const auto& proj = libgraphics::ComputeCameraProjection(60.0, gl_context->Data().m_width, gl_context->Data().m_height, 0.01, 1000.0);

		const auto inverse_view_projection = glm::inverse(proj * view);
		const auto screen_pos = glm::vec4{ mouse_screen_ndc_coords.x, -mouse_screen_ndc_coords.y, 1.0f, 1.0f };
		const auto world_pos = inverse_view_projection * screen_pos;

		const auto ray_origin = m_transform_component->GetLocalTranslation();
		const auto ray_direction = glm::normalize(glm::vec3{ world_pos });

		return { ray_origin, ray_direction };
	}

	auto Camera::InternalTranslate(const CameraDirection direction, const float delta_time) const -> void
	{
		const auto [front, right, up] = CalculateDirectionVectors();
		switch (direction)
		{
		case (CameraDirection::front):	m_transform_component->Translate(front * 15.0f * delta_time); break;
		case (CameraDirection::back):	m_transform_component->Translate(-front * 15.0f * delta_time); break;
		case (CameraDirection::left):	m_transform_component->Translate(-right * 15.0f * delta_time); break;
		case (CameraDirection::right):	m_transform_component->Translate(right * 15.0f * delta_time); break;
		case (CameraDirection::up):		m_transform_component->Translate(up * 15.0f * delta_time); break;
		case (CameraDirection::down):	m_transform_component->Translate(-up * 15.0f * delta_time); break;
		default:break;
		}
	}

	void Camera::Initialize()
	{
		Component::Initialize();

		m_transform_component = GetEntity().GetTransformComponent();
	}

	void Camera::Update(const float delta_time)
	{
		Component::Update(delta_time);

		const auto& core = Core::GetInstance();
		const auto gl_context = ::std::static_pointer_cast<GLContext>(core.GetGraphicsWindow()->GetNativeHandle());

		if (const auto glfw_window = static_cast<GLFWwindow*>(gl_context->GetNativeHandle()); glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_2))
		{
			RotateByMouse(core.GetGraphicsWindow());
		}
		else
		{
			Reset();
		}

		Animate(core.GetGraphicsWindow(), delta_time);
	}
}
