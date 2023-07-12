#include <components/transform.h>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace libgraphics
{
	void Transform::Initialize()
	{
		m_local_translation = {};
		m_local_scale = glm::vec3{ 1.0f };
		m_local_rotation = {};
	}

	auto Transform::Translate(const glm::vec3& delta) -> void
	{
		m_local_translation += delta;
		m_is_dirty = true;
	}

	auto Transform::SetLocalTranslation(const glm::vec3& translation) -> void
	{
		m_local_translation = translation;
		m_is_dirty = true;
	}

	auto Transform::SetLocalRotation(const glm::vec3& rotation) -> void
	{
		glm::vec3 scale;
		glm::quat orientation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(m_model_matrix, scale, orientation, translation, skew, perspective);

		const glm::mat4 rotation_matrix = glm::eulerAngleXYZ(glm::radians(rotation.x), glm::radians(rotation.y), glm::radians(rotation.z));
		m_local_orientation = glm::quat_cast(rotation_matrix);

		m_local_rotation = rotation;

		m_is_dirty = true;
	}

	auto Transform::Reset() -> void
	{
		m_local_translation = {};
		m_local_scale = glm::vec3{ 1.0f };
		m_local_rotation = {};
		m_is_dirty = true;
	}

	auto Transform::GetLocalModelMatrix() const -> glm::mat4
	{
		const auto& translation_matrix = glm::translate(glm::identity<glm::mat4>(), m_local_translation);
		const auto& rotation_matrix = glm::mat4_cast(m_local_orientation);
		const auto& scale_matrix = glm::scale(glm::identity<glm::mat4>(), m_local_scale);

		const auto& local_transform = translation_matrix * rotation_matrix * scale_matrix;

		return local_transform;
	}

	auto Transform::ComputeModelMatrix() -> void
	{
		m_model_matrix = GetLocalModelMatrix();
		m_is_dirty = false;
	}

	auto Transform::ComputeModelMatrix(const glm::mat4& parent_global_model_matrix) -> void
	{
		m_model_matrix = parent_global_model_matrix * GetLocalModelMatrix();
		m_is_dirty = false;
	}
}
