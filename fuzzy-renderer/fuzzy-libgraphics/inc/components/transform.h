#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include <components/component.h>

namespace libgraphics
{
	class Transform : public Component
	{
	public:
		Transform() = default;

		void Initialize() override
		{
			m_translation = glm::vec3(0.0f);
			m_scale = glm::vec3(1.0f);
			m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		}

		auto SetTranslation(const glm::vec3& translation) { m_translation = translation; }
		[[nodiscard]] auto& GetTranslation() const { return m_translation; }

		auto SetScale(const glm::vec3& scale) { m_scale = scale; }
		[[nodiscard]] auto& GetScale() const { return m_scale; }

		auto SetRotation(const glm::quat& rotation) { m_rotation = rotation; }
		[[nodiscard]] auto& GetRotation() const { return m_rotation; }

		[[nodiscard]] auto GetEulerAngles() const
		{
			const auto& euler_angles = glm::eulerAngles(m_rotation);
			return glm::degrees(euler_angles);
		}

		auto RotateAroundAxis(const glm::vec3& axis, float angle)
		{
			const auto& rotation = glm::angleAxis(angle, axis);
			m_rotation = rotation * m_rotation;
		}

		auto Reset()
		{
			m_translation = glm::vec3(0.0f);
			m_scale = glm::vec3(1.0f);
			m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		}

	private:
		glm::vec3 m_translation = {};
		glm::vec3 m_scale = {};
		glm::quat m_rotation = {};
	};

}
