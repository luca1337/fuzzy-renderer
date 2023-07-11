#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include <components/component.h>
#include <glm/gtx/rotate_vector.hpp>

namespace libgraphics
{
	class Transform : public Component
	{
	public:
		Transform() = default;

		void Initialize() override;

		auto SetLocalTranslation(const glm::vec3& translation) -> void;
		auto SetLocalScale(const glm::vec3& scale) { m_local_scale = scale; m_is_dirty = true; }
		auto SetLocalRotation(const glm::vec3& rotation) -> void;
		[[nodiscard]] auto& GetLocalTranslation() const { return m_local_translation; }
		[[nodiscard]] auto& GetLocalScale() const { return m_local_scale; }
		[[nodiscard]] auto& GetLocalRotation() const { return m_local_rotation; }

		auto Reset() -> void;

		[[nodiscard]] auto GetLocalModelMatrix() const->glm::mat4;
		[[nodiscard]] auto GetWorldModelMatrix() const { return m_model_matrix; }
		[[nodiscard]] auto IsDirty() const { return m_is_dirty; }

		auto ComputeModelMatrix() -> void;
		auto ComputeModelMatrix(const glm::mat4& parent_global_model_matrix) -> void;

	private:
		glm::vec3 m_local_translation = {};
		glm::vec3 m_local_scale = {};
		glm::vec3 m_local_rotation = {};

		glm::quat m_local_orientation = {};

		glm::mat4 m_model_matrix = glm::identity<glm::mat4>();

		bool m_is_dirty = true;

		/*[[nodiscard]] auto CalculateDirectionVectors() const -> std::tuple<glm::dvec3, glm::dvec3, glm::dvec3>
		{
			glm::dvec3 direction = {};
			direction.x = sin(glm::radians(m_local_rotation.y)) * cos(glm::radians(m_local_rotation.x));
			direction.y = sin(glm::radians(m_local_rotation.x));
			direction.z = cos(glm::radians(m_local_rotation.y)) * cos(glm::radians(m_local_rotation.x));
			const auto front = glm::normalize(direction);

			constexpr glm::dvec3 world_up = { 0.0, 1.0, 0.0 };

			const auto right = glm::normalize(glm::cross(front, world_up));
			const auto up = glm::normalize(glm::cross(right, front));

			return { front, right, up };
		}*/
	};
}
