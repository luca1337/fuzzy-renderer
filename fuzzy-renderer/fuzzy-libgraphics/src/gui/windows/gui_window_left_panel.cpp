#include <gui_utils.h>
#include <glm/gtc/type_ptr.hpp>
#include <gui/windows/gui_window_left_panel.h>

#include <core.h>
#include <entity_manager.h>
#include <iostream>
#include <ostream>
#include <components/mesh_renderer.h>
#include <entities/entity.h>
#include <interfaces/igraphics_context.h>

namespace libgraphics::gui
{
	GUIWindowLeftPanel::GUIWindowLeftPanel()
	{
		m_title = "Stats Window";
		m_bg_alpha = 0.75f;
		m_size = { 370.0f, 0.0f };
		m_position = { 0.0f, 18.0f };
		m_flags = ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoFocusOnAppearing |
			ImGuiWindowFlags_NoNav |
			ImGuiWindowFlags_NoMove;
	}

	bool DragFloatWithLabel(const std::string& label, float* value, const ImVec4& color, const std::string& operation, const bool same_line_next_item)
	{
		const std::string unique_id = "##" + operation + label;
		ImGui::PushID(unique_id.c_str());
		utils::gui::ColoredText(label, color);
		ImGui::SameLine();
		ImGui::PushItemWidth(50);
		const auto value_changed = ImGui::DragFloat(unique_id.c_str(), value, 0.01f, -1000.0f, 1000.0f);
		ImGui::PopItemWidth();
		if (same_line_next_item) ImGui::SameLine();
		ImGui::PopID();

		return value_changed;
	}

	std::shared_ptr<Entity> selected_entity = {};

	void DrawEntityHierarchy(const std::shared_ptr<Entity>& root_entity, bool is_root = true)
	{
		if (const bool has_children = !root_entity->GetChildrens().empty(); is_root || has_children)
		{
			const bool tree_node_open = ImGui::TreeNode(root_entity->GetName().data());

			if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			{
				selected_entity = root_entity;
			}

			if (tree_node_open)
			{
				for (const auto& childrens = root_entity->GetChildrens(); const auto & child : childrens)
				{
					DrawEntityHierarchy(child, false);
				}

				ImGui::TreePop();
			}
		}
		else
		{
			if (ImGui::Selectable(root_entity->GetName().data(), selected_entity == root_entity))
			{
				selected_entity = root_entity;
			}
		}
	}

	void GUIWindowLeftPanel::Render()
	{
		const auto& core = Core::GetInstance();
		const auto& window = core.GetGraphicsWindow()->GetNativeHandle();

		utils::gui::RenderWindowContent(m_title, m_is_open, m_size, m_position, m_flags, m_bg_alpha, [&] {
			if (ImGui::BeginTabBar("LeftPanel"))
			{
				if (ImGui::BeginTabItem("Scene - Entities"))
				{
					for (const auto& entity_mgr = core.GetEntityManager(); const auto & root_entity : entity_mgr->GetEntities())
					{
						DrawEntityHierarchy(root_entity, root_entity->GetChildrens().empty());
					}

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		});

		const auto entity_window_props_position = ImVec2{ window->Data().m_width - m_size.x, m_position.y };

		utils::gui::RenderWindowContent("Entity Inspector", m_is_open, m_size, entity_window_props_position, m_flags, m_bg_alpha, [&] {

			if (ImGui::BeginTabBar("Props"))
			{
				if (ImGui::BeginTabItem("Entity"))
				{
					if (selected_entity)
					{
						// Transform is always present in a default component and cannot be removed!
						ImGui::Spacing();
						if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen))
						{
							ImGui::Spacing();

							if (const auto& transform_component = selected_entity->GetTransformComponent())
							{
								auto translation = transform_component->GetLocalTranslation();

								ImGui::BulletText("Translation");
								if (DragFloatWithLabel("X", &translation.x, utils::gui::ColorRed, "Translation", true))
								{
									transform_component->SetLocalTranslation(translation);
								}

								if (DragFloatWithLabel("Y", &translation.y, utils::gui::ColorGreen, "Translation", true))
								{
									transform_component->SetLocalTranslation(translation);
								}

								if (DragFloatWithLabel("Z", &translation.z, utils::gui::ColorBlue, "Translation", false))
								{
									transform_component->SetLocalTranslation(translation);
								}


								ImGui::Spacing();

								auto rotation = transform_component->GetLocalRotation();

								ImGui::BulletText("Rotation");
								if (DragFloatWithLabel("X", &rotation.x, utils::gui::ColorRed, "Rotation", true))
								{
								transform_component->SetLocalRotation(rotation);
								}
								if (DragFloatWithLabel("Y", &rotation.y, utils::gui::ColorGreen, "Rotation", true))
								{
									transform_component->SetLocalRotation(rotation);
								}
								if (DragFloatWithLabel("Z", &rotation.z, utils::gui::ColorBlue, "Rotation", false))
								{
									transform_component->SetLocalRotation(rotation);
								}
								
								ImGui::Spacing();

								auto scale = transform_component->GetLocalScale();

								ImGui::BulletText("Scale");
								if (DragFloatWithLabel("X", &scale.x, utils::gui::ColorRed, "Scale", true))
								{
									transform_component->SetLocalScale(scale);
								}
								if (DragFloatWithLabel("Y", &scale.y, utils::gui::ColorGreen, "Scale", true))
								{
									transform_component->SetLocalScale(scale);
								}
								if (DragFloatWithLabel("Z", &scale.z, utils::gui::ColorBlue, "Scale", false))
								{
									transform_component->SetLocalScale(scale);
								}
							}

							ImGui::TreePop();
						}

						ImGui::Spacing();

						if (ImGui::TreeNodeEx("MeshRenderer", ImGuiTreeNodeFlags_DefaultOpen))
						{
							ImGui::Spacing();
							if (const auto& mesh_renderer_component = selected_entity->GetComponent<MeshRenderer>())
							{
								// List materials, lights etc etc
								if (ImGui::TreeNode("Materials"))
								{
									ImGui::Spacing();

									const auto& material = mesh_renderer_component->GetMaterial();

									ImGui::BulletText("Albedo"); ImGui::SameLine();

									if (ImGui::RadioButton("##Albedo", true))
									{
										// implement logic to load albedo from path
									}

									ImGui::SameLine();

									if (auto albedo_color = material->GetAlbedoColor(); ImGui::ColorEdit3("##Albedo", glm::value_ptr(albedo_color)))
									{
										material->SetAlbedoColor(albedo_color);
									}

									ImGui::BulletText("Metallic"); ImGui::SameLine();
									if (auto metallic = material->GetMetallic(); ImGui::SliderFloat("##Metallic", &metallic, 0.0f, 1.0f))
									{
										material->SetMetallic(metallic);
									}

									ImGui::BulletText("Roughness"); ImGui::SameLine();
									if (auto roughness = material->GetRoughness(); ImGui::SliderFloat("##Roughness", &roughness, 0.0f, 1.0f))
									{
										material->SetRoughness(roughness);
									}

									ImGui::TreePop();
								}
							}

							ImGui::TreePop();
						}
					}

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
			});
	}
}
