#include <core.h>
#include <utils.h>
#include <glad/gl.h>
#include <interfaces/ishader.h>
#include <opengl/gl_context.h>
#include <opengl/gl_skybox.h>

namespace libgraphics
{
    constexpr float skybox_vertices[] = 
    {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

	GLSkybox::GLSkybox()
	{
        Setup();
		m_cubemap_tex_id = utils::gl::LoadCubemap("../fuzzy-libgraphics/cubemaps/sky_01");
	}

	auto GLSkybox::Render(const std::shared_ptr<IShader>& shader) const -> void
	{
		const auto& core = Core::GetInstance();
		const auto gl_context = ::std::static_pointer_cast<GLContext>(core.GetGraphicsWindow()->GetNativeHandle());

        const auto& native_camera = core.GetMainCamera()->GetNativeCamera();

        glDepthMask(GL_FALSE);
		shader->Bind();
        shader->SetMatrix4x4("view", native_camera->GetViewMatrix3());
        shader->SetFloat("time", core.GetDeltaTime());
		shader->SetMatrix4x4("projection", ComputeCameraProjection(native_camera->Props().m_fov, gl_context->Data().m_width, gl_context->Data().m_height, native_camera->Props().m_z_near, native_camera->Props().m_z_far));

		glBindVertexArray(m_sky_vao);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemap_tex_id);
		glDrawArrays(GL_TRIANGLES, 0, utils::common::ArraySize(skybox_vertices) / 3);
		glBindVertexArray(0);
        glDepthMask(GL_TRUE);
	}

	auto GLSkybox::Setup() -> void
	{
        // skybox VAO
        glGenVertexArrays(1, &m_sky_vao);
        glGenBuffers(1, &m_sky_vbo);
        glBindVertexArray(m_sky_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_sky_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof skybox_vertices, &skybox_vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
	}
}
