#include <core.h>
#include <components/mesh_renderer.h>
#include <components/transform.h>
#include <opengl/gl_context.h>

#include <glm/gtx/transform.hpp>

#include "enums.h"
#include "resource_manager.h"
#include "entities/entity.h"

namespace libgraphics
{
	auto MeshRenderer::Initialize() -> void
	{
		// todo: shader should become shader : component

		const auto& default_shader = libgraphics::ResourceManager::GetFromCache<GLShader>({ libgraphics::ResourceType::shaders, "default_shader" });

		// sets default shader
		SetShader(default_shader.value());
	}

	auto MeshRenderer::Render() -> void
	{
		m_mesh.Draw(m_shader);
	}

	auto MeshRenderer::Update(float delta_time) -> void
	{
		UpdateMatrix(m_shader);
	}

	auto MeshRenderer::UpdateMatrix(const std::shared_ptr<IShader>& shader) const -> void
	{
		const auto& transform_component = GetEntity().GetTransformComponent();

		const auto& core = Core::GetInstance();
		const auto gl_context = ::std::static_pointer_cast<GLContext>(core.GetGraphicsWindow()->GetNativeHandle());

		const auto& translation_mat = glm::translate(transform_component.GetTranslation());
		const auto& scale_mat = glm::scale(transform_component.GetScale());
		const auto& rotation_mat = glm::mat4_cast(transform_component.GetRotation());

		const auto& model = translation_mat * rotation_mat * scale_mat;
		const auto& view = GetViewMatrix(core.GetMainCamera().m_camera_props);
		const auto& projection = ComputeCameraProjection(60.0, gl_context->Data().m_width, gl_context->Data().m_height, 0.01, 1000.0);
		const auto& eye = core.GetMainCamera().GetWorldPosition();

		shader->SetMatrix4x4("model", model);
		shader->SetMatrix4x4("view", view);
		shader->SetMatrix4x4("projection", projection);
		shader->SetVec3("eye", eye);
	}
}
