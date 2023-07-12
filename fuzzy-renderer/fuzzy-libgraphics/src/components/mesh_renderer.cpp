#include <core.h>
#include <enums.h>
#include <resource_manager.h>
#include <utils.h>
#include <components/mesh_renderer.h>
#include <components/transform.h>
#include <entities/entity.h>
#include <opengl/gl_context.h>
#include <rendering/texture.h>

#include "components/camera.h"

namespace libgraphics
{
	auto MeshRenderer::Initialize() -> void
	{
		const auto& default_shader = libgraphics::ResourceManager::GetFromCache<GLShader>({ libgraphics::ResourceType::shaders, "default_shader" });
		SetShader(default_shader.value());

		m_shader->Bind();

		m_default_material = std::make_shared<lighting::Material>();
		m_default_material->SetMetallic(0.3f);
		m_default_material->SetRoughness(0.2f);
	}

	auto MeshRenderer::Render() -> void
	{
		m_shader->Bind();

		auto textures = m_mesh.GetTextures();

		if (!textures.empty())
		{
			std::ranges::for_each(std::views::iota(0ul) | std::views::take(textures.size()), [&](const int texture_idx) {
				glActiveTexture(GL_TEXTURE0 + texture_idx);

				auto& texture = textures[texture_idx];
				const auto& texture_type = texture.GetType();
				texture.SetIndex(texture_idx);

				if (texture_type == TextureType::albedo) { m_default_material->SetAlbedoMap(texture); }
				else if (texture_type == TextureType::specular) { m_default_material->SetMetallicMap(texture); }
				else if (texture_type == TextureType::normals) { m_default_material->SetNormalMap(texture); }

				glBindTexture(GL_TEXTURE_2D, texture.GetTextureID());
			});

			// todo: support other textures
			m_shader->SetInt("material.albedo_map", m_default_material->GetAlbedoMap()->GetIndex());
			m_shader->SetInt("material.metallic_map", m_default_material->GetMetallicMap()->GetIndex());
			m_shader->SetInt("material.normal_map", m_default_material->GetNormalMap()->GetIndex());
		}
		else
		{
			m_shader->SetVec3("material.albedo_color", m_default_material->GetAlbedoColor());
			m_shader->SetVec3("material.emission_color", m_default_material->GetEmissionColor());
		}

		m_shader->SetFloat("material.metallic", m_default_material->GetMetallic());
		m_shader->SetFloat("material.roughness", m_default_material->GetRoughness());
		m_shader->SetBool("material.use_textures", !textures.empty());

		UpdateMatrix();

		/*const auto& core = Core::GetInstance();
		const auto gl_context = ::std::static_pointer_cast<GLContext>(core.GetGraphicsWindow()->GetNativeHandle());

		double mouseX, mouseY;
		glfwGetCursorPos(static_cast<GLFWwindow*>(gl_context->GetNativeHandle()), &mouseX, &mouseY);

		const auto ray = core.GetMainCamera().ScreenPointToRay3D(glm::vec2(mouseX, mouseY));
		if (const auto ray_hit = utils::gl::CheckRayMeshIntersection(core.GetMainCamera().GetWorldPosition(), ray.m_direction, m_mesh))
		{
			if (ray_hit.has_value())
			{
				const auto& hit_point = ray_hit.value();

				const std::string output = std::format("Ray hit at distance: {} and hit point: ({}, {}, {})", hit_point.m_distance, hit_point.m_hit_point.x, hit_point.m_hit_point.y, hit_point.m_hit_point.z);
				CX_CORE_DEBUG(output);
			}
		}*/

		m_mesh.Draw(m_shader);
	}

	auto MeshRenderer::UpdateMatrix() const -> void
	{
		const auto& transform_component = GetEntity().GetTransformComponent();

		const auto& core = Core::GetInstance();
		const auto gl_context = ::std::static_pointer_cast<GLContext>(core.GetGraphicsWindow()->GetNativeHandle());

		// Get camera
		const auto& native_camera = core.GetMainCamera()->GetNativeCamera();

		const auto& view = native_camera->GetViewMatrix();
		const auto& projection = ComputeCameraProjection(native_camera->Props().m_fov, gl_context->Data().m_width, gl_context->Data().m_height, native_camera->Props().m_z_near, native_camera->Props().m_z_far);
		const auto& eye = native_camera->GetEntity().GetTransformComponent()->GetLocalTranslation();

		m_shader->SetMatrix4x4("model", transform_component->GetWorldModelMatrix());
		m_shader->SetMatrix4x4("view", view);
		m_shader->SetMatrix4x4("projection", projection);
		m_shader->SetVec3("eye", eye);
	}
}
