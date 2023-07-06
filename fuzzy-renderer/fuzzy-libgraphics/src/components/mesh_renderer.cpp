#include <core.h>
#include <enums.h>
#include <resource_manager.h>
#include <components/mesh_renderer.h>
#include <components/transform.h>
#include <entities/entity.h>
#include <glm/gtx/transform.hpp>
#include <opengl/gl_context.h>
#include <rendering/texture.h>

namespace libgraphics
{
	auto MeshRenderer::Initialize() -> void
	{
		const auto& default_shader = libgraphics::ResourceManager::GetFromCache<GLShader>({ libgraphics::ResourceType::shaders, "default_shader" });
		SetShader(default_shader.value());

		m_shader->Bind();

		auto textures = m_mesh.GetTextures();

		m_material = std::make_shared<lighting::Material>();
		m_material->m_metallic = 32.0f;
		m_material->m_roughness = 0.5f;
		m_material->m_use_textures = !textures.empty();

		if (!textures.empty())
		{
			std::ranges::for_each(std::views::iota(0ul) | std::views::take(textures.size()), [&](const int texture_idx) {
				glActiveTexture(GL_TEXTURE0 + texture_idx);

				auto& texture = textures[texture_idx];
				const auto texture_type = texture.GetType();

				texture.SetIndex(texture_idx);

				if (texture_type == TextureType::albedo) { m_material->m_albedo_map = texture.GetIndex(); }
				else if (texture_type == TextureType::specular) { m_material->m_specular_map = texture.GetIndex(); }
				else if (texture_type == TextureType::normals) { m_material->m_normal_map = texture.GetIndex(); }
				else if (texture_type == TextureType::height) { m_material->m_height_map = texture.GetIndex(); }
				else if (texture_type == TextureType::opacity) { m_material->m_opacity_map = texture.GetIndex(); }

				glBindTexture(GL_TEXTURE_2D, texture.GetTextureID());
			});

			// todo: support other textures
			m_shader->SetInt("material.albedo_map", m_material->m_albedo_map);
			m_shader->SetInt("material.metallic_map", m_material->m_specular_map);
			m_shader->SetInt("material.normal_map", m_material->m_normal_map);
		}
		else
		{
			m_shader->SetVec3("material.albedo_color", m_material->m_albedo_color);
			m_shader->SetVec3("material.emission_color", m_material->m_emission_color);
		}

		m_shader->SetFloat("material.metallic", m_material->m_metallic);
		m_shader->SetFloat("material.roughness", m_material->m_roughness);
		m_shader->SetBool("material.use_textures", m_material->m_use_textures);
	}

	auto MeshRenderer::Render() -> void
	{
		m_shader->Bind();

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
