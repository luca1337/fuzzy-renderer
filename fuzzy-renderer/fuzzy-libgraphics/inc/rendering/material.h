#pragma once

#include <rendering/texture.h>
#include <optional>

namespace libgraphics::lighting
{
	class Material
	{
	public:
		Material() = default;

		[[ nodiscard ]] auto GetName() const -> const std::string& { return m_name; }
		[[ nodiscard ]] auto& GetAlbedoMap() const { return m_albedo_map; }
		[[ nodiscard ]] auto& GetMetallicMap() const { return m_metallic_map; }
		[[ nodiscard ]] auto& GetNormalMap() const { return m_normal_map; }
		[[ nodiscard ]] auto GetMetallic() const -> float { return m_metallic; }
		[[ nodiscard ]] auto GetRoughness() const -> float { return m_roughness; }
		[[ nodiscard ]] auto GetOcclusionStrength() const -> float { return m_occlusionStrength; }
		[[ nodiscard ]] auto GetEmissionStrength() const -> float { return m_emissionStrength; }
		[[ nodiscard ]] auto GetAlbedoColor() const -> const glm::vec3& { return m_albedoColor; }
		[[ nodiscard ]] auto GetEmissionColor() const -> const glm::vec3& { return m_emissionColor; }
		[[ nodiscard ]] auto UseTextures() const -> bool { return m_useTextures; }

		auto SetName(const std::string& name) -> void { m_name = name; }
		auto SetAlbedoMap(const Texture& albedo) -> void { m_albedo_map = albedo; }
		auto SetMetallicMap(const Texture& metallic) -> void { m_metallic_map = metallic; }
		auto SetNormalMap(const Texture& normal) -> void { m_normal_map = normal; }
		auto SetMetallic(const float metallic) -> void { m_metallic = glm::clamp(metallic, 0.0f, 1.0f); }
		auto SetRoughness(const float roughness) -> void { m_roughness = glm::clamp(roughness, 0.0f, 1.0f); }
		auto SetOcclusionStrength(const float occlusion_strength) -> void { m_occlusionStrength = occlusion_strength; }
		auto SetEmissionStrength(const float emission_strength) -> void { m_emissionStrength = emission_strength; }
		auto SetAlbedoColor(const glm::vec3& albedo_color) -> void { m_albedoColor = albedo_color; }
		auto SetEmissionColor(const glm::vec3& emission_color) -> void { m_emissionColor = emission_color; }
		auto SetUseTextures(const bool use_textures) -> void { m_useTextures = use_textures; }

	private:
		std::string m_name = {};
		std::optional<Texture> m_albedo_map = {};
		std::optional<Texture> m_metallic_map = {};
		std::optional<Texture> m_normal_map = {};
		float m_metallic = {};
		float m_roughness = {};
		float m_occlusionStrength = {};
		float m_emissionStrength = {};
		glm::vec3 m_albedoColor = glm::vec3{ 1.0f };
		glm::vec3 m_emissionColor = glm::vec3{ 1.0f };
		bool m_useTextures = {};
	};
}
