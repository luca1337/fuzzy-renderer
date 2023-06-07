#include <camera.h>

#include <gl_window.h>
#include <gl_context.h>

#include <GLFW/glfw3.h>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace libgraphics
{
	auto GetViewMatrix(const CameraProps& cameraProps) -> glm::mat4
	{
		const auto [front, right, up] = CalculateDirectionVectors(cameraProps);
		return glm::lookAt(cameraProps.worldPosition, cameraProps.worldPosition + front, up);
	}

	auto ComputeCameraProjection(const double fov, const double width, const double height, const double zNear, const double zFar) -> glm::mat4
	{
		return glm::perspective(glm::radians(fov), width / height, zNear, zFar);
	}

	auto CalculateDirectionVectors(const CameraProps& camera_props) -> std::tuple<glm::dvec3, glm::dvec3, glm::dvec3>
	{
		glm::dvec3 direction = {};
		direction.x = cos(glm::radians(camera_props.yaw)) * cos(glm::radians(camera_props.pitch));
		direction.y = -sin(glm::radians(camera_props.pitch));
		direction.z = sin(glm::radians(camera_props.yaw)) * cos(glm::radians(camera_props.pitch));
		const auto front = glm::normalize(direction);

		constexpr glm::dvec3 world_up = { 0.0, 1.0, 0.0 };

		const auto right = glm::normalize(glm::cross(front, world_up));
		const auto up = glm::normalize(glm::cross(right, front));

		return { front, right, up };
	}

	auto Camera::GetWorldPosition() const -> glm::dvec3
	{
		return glm::dvec3();
	}

	bool first_mouse = true;
	double lastX = {}, lastY = {};
	double xPos = {}, yPos = {};
	auto Camera::RotateByMouse(std::shared_ptr<GLWindow>& window) -> void
	{
		const auto glfwNativeWindowHandle = reinterpret_cast<GLFWwindow*>(window->GetNativeHandle()->GetNativeHandle());

		glfwGetCursorPos(glfwNativeWindowHandle, &xPos, &yPos);

		const auto delta = (glm::dvec2(xPos, yPos) - _previousPosition.value_or(glm::dvec2(xPos, yPos))) * 0.1;
		_previousPosition = glm::dvec2(xPos, yPos);

		CameraProps.yaw += delta.x;
		CameraProps.pitch += delta.y;

		// make sure that when pitch is out of bounds, screen doesn't get flipped.
		constexpr auto max_pitch = 89.9f;
		if (CameraProps.pitch >= max_pitch)	CameraProps.pitch = max_pitch;
		if (CameraProps.pitch <= -max_pitch) CameraProps.pitch = -max_pitch;
	}

	auto Camera::Animate(std::shared_ptr<GLWindow>& window, const double deltaTime) -> void
	{
		const auto glfwNativeWindowHandle = reinterpret_cast<GLFWwindow*>(window->GetNativeHandle()->GetNativeHandle());

		if (glfwGetKey(glfwNativeWindowHandle, GLFW_KEY_W))
		{
			Translate(CameraDirection::Front, deltaTime);
		}
		if (glfwGetKey(glfwNativeWindowHandle, GLFW_KEY_S))
		{
			Translate(CameraDirection::Back, deltaTime);
		}
		if (glfwGetKey(glfwNativeWindowHandle, GLFW_KEY_A))
		{
			Translate(CameraDirection::Left, deltaTime);
		}
		if (glfwGetKey(glfwNativeWindowHandle, GLFW_KEY_D))
		{
			Translate(CameraDirection::Right, deltaTime);
		}
		if (glfwGetKey(glfwNativeWindowHandle, GLFW_KEY_E))
		{
			Translate(CameraDirection::Up, deltaTime);
		}
		if (glfwGetKey(glfwNativeWindowHandle, GLFW_KEY_Q))
		{
			Translate(CameraDirection::Down, deltaTime);
		}
	}

	auto Camera::Translate(const CameraDirection direction, const double deltaTime) -> void
	{
		const auto [front, right, up] = CalculateDirectionVectors(CameraProps);
		switch (direction)
		{
		case (CameraDirection::Front):	CameraProps.worldPosition += front * 15.0 * deltaTime; break;
		case (CameraDirection::Back):	CameraProps.worldPosition -= front * 15.0 * deltaTime; break;
		case (CameraDirection::Left):	CameraProps.worldPosition -= right * 15.0 * deltaTime; break;
		case (CameraDirection::Right):	CameraProps.worldPosition += right * 15.0 * deltaTime; break;
		case (CameraDirection::Up):		CameraProps.worldPosition += up * 15.0 * deltaTime; break;
		case (CameraDirection::Down):	CameraProps.worldPosition -= up * 15.0 * deltaTime; break;
		default:break;
		}
	}
}