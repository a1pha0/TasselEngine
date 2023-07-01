#include "Tassel/Core/Base.h"
#include "Tassel/Scene/SceneHierarchyPanel.h"
#include "Tassel/Scene/SceneSerializer.h"
#include "Tassel/Scene/Component.h"

namespace Tassel
{
	SceneHierarchyPanel::SceneHierarchyPanel(Scene* scene)
	{
		SetScene(scene);
	}

	SceneHierarchyPanel::~SceneHierarchyPanel()
	{

	}

	void SceneHierarchyPanel::SetScene(Scene* scene)
	{
		m_Scene = scene;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy Panel");

		//ImGui::Text("Entity Count: %d", m_Scene->m_Registry.alive());

		auto view = m_Scene->m_Registry.view<TagComponent>();

		for (entt::entity entityID : view)
		{
			Entity entity = { entityID, m_Scene };
			DrawEntityNode(entity);
		}

		//if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		//	m_Scene->m_SelectedEntity = {};

		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("New Entity"))
				m_Scene->m_SelectedEntity = m_Scene->CreateEntity("New Entity");
			ImGui::EndPopup();
		}
		ImGui::End();

		DrawDetails(m_Scene->m_SelectedEntity);
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity& entity)
	{
		const char* tag = entity.GetComponent<TagComponent>().Tag.c_str();

		ImGuiTreeNodeFlags flags = ((m_Scene->m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		bool opened = ImGui::TreeNodeEx((const void*)(entity.GetEntityID()), flags, tag);

		if (ImGui::IsItemClicked())
			m_Scene->m_SelectedEntity = entity;

		bool deleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				if (m_Scene->m_SelectedEntity == entity)
					m_Scene->m_SelectedEntity = {};
				deleted = true;
			}
			ImGui::EndPopup();
		}

		if (opened)
			ImGui::TreePop();

		if (deleted)
			m_Scene->DestoryEntity(entity);
	}


	static void DrawVec3Control(const char* label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGui::PushID(label);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label);

		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.6f, 0.1f, 0.1f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.8f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.7f, 0.1f, 0.1f, 1.0f });
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.6f, 0.1f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.8f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.6f, 0.1f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.1f, 0.6f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.2f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.1f, 0.6f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::PopID();
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(Entity& entity, const std::string& label, UIFunction uiFunction)
	{
		if (entity.HasComponent<T>())
		{
			const void* id = (const void*)typeid(T).hash_code();

			ImGui::PushID(id);

			static ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding;

			if (ImGui::Button("+"))
				ImGui::OpenPopup("Component Setting");

			bool removed = false;
			if (ImGui::BeginPopup("Component Setting"))
			{
				if (ImGui::MenuItem("Remove Component"))
					removed = true;
				ImGui::EndPopup();
			}
			ImGui::SameLine();
			bool opened = ImGui::TreeNodeEx(id, treeNodeFlags, label.c_str());

			if (opened)
			{
				auto& component = entity.GetComponent<T>();
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removed)
				entity.RemoveComponent<T>();

			ImGui::PopID();
		}
	}

	void SceneHierarchyPanel::DrawDetails(Entity& entity)
	{
		ImGui::Begin("Details");
		if (entity && entity.HasComponent<TagComponent>())
		{
			std::string& tag = entity.GetComponent<TagComponent>().Tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
				tag = buffer;

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("Add Component");

			if (ImGui::BeginPopup("Add Component"))
			{
				if (ImGui::MenuItem("Camera"))
				{
					entity.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Sprite Renderer"))
				{
					entity.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Circle Mesh"))
				{
					entity.AddComponent<CircleMeshComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Rigid Body 2D"))
				{
					entity.AddComponent<RigidBody2DComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Box Collider 2D"))
				{
					entity.AddComponent<BoxCollider2DComponent>();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			DrawComponent<TransformComponent>(entity, "Transform Component", [](TransformComponent& transformComp)
				{
					DrawVec3Control("Position", transformComp.Translation);
					glm::vec3 rotation = glm::degrees(transformComp.Rotation);
					DrawVec3Control("Rotation", rotation);
					transformComp.Rotation = glm::radians(rotation);
					DrawVec3Control("Scale", transformComp.Scale, 1.0f);
				});

			DrawComponent<SpriteRendererComponent>(entity, "Sprite Renderer Component", [](SpriteRendererComponent& component)
				{
					ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

					ImGui::Button("Texture", ImVec2(128.0f, 0.0f));
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
						{
							delete component.Texture;
							component.Texture = Texture2D::Create((const char*)payload->Data);
						}
						ImGui::EndDragDropTarget();
					}
				});

			DrawComponent<CircleMeshComponent>(entity, "Circle Mesh Component", [](CircleMeshComponent& component)
				{
					ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
					DrawVec3Control("Offset", component.Offset, 0.1f);
					ImGui::DragFloat("Randius", &component.Randius, 0.1f);
					
					int edges = component.Edges;
					ImGui::DragInt("Edges", &edges);
					component.Edges = edges;

					ImGui::DragFloat("Thickness", &component.Thickness, 0.1f);
					ImGui::DragFloat("Shade", & component.Shade, 0.1f);
				});

			DrawComponent<CameraComponent>(entity, "Camera Component", [](CameraComponent& cameraComp)
				{
					Camera& camera = cameraComp.Camera;

					ImGui::Checkbox("Primary", &cameraComp.Primary);

					const char* projectionTypes[] = { "Perspective", "Orthographic" };
					const char* currentPeojectionType = projectionTypes[(int)camera.GetProjectionType()];

					if (ImGui::BeginCombo("Projection Type", currentPeojectionType))
					{
						for (int i = 0; i < 2; i++)
						{
							bool selected = projectionTypes[i] == currentPeojectionType;
							if (ImGui::Selectable(projectionTypes[i], &selected))
							{
								currentPeojectionType = projectionTypes[i];
								camera.SetProjectionType((Camera::ProjectionType)i);
							}

							if (selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}

					if (camera.GetProjectionType() == Camera::ProjectionType::Perspective)
					{
						float fov = camera.GetPerspectiveFOV();
						if (ImGui::DragFloat("FOV", &fov, 0.1f))
							camera.SetPerspectiveFOV(fov);

						float nearClip = camera.GetPerspectiveNearClip();
						if (ImGui::DragFloat("Near Clip", &nearClip, 0.1f))
							camera.SetPerspectiveNearClip(nearClip);

						float farClip = camera.GetPerspectiveFarClip();
						if (ImGui::DragFloat("Far Clip", &farClip, 0.1f))
							camera.SetPerspectiveFarClip(farClip);

						ImGui::Text("Aspect Ratio: %.3f", camera.GetAspectRatio());

					}
					else if (camera.GetProjectionType() == Camera::ProjectionType::Orthographic)
					{
						float size = camera.GetOrthographicSize();
						if (ImGui::DragFloat("Orthographic Size", &size, 0.1f))
							camera.SetOrthographicSize(size);

						float nearClip = camera.GetOrthographicNearClip();
						if (ImGui::DragFloat("Near Clip", &nearClip, 0.1f))
							camera.SetOrthographicNearClip(nearClip);

						float farClip = camera.GetOrthographicFarClip();
						if (ImGui::DragFloat("Far Clip", &farClip, 0.1f))
							camera.SetOrthographicFarClip(farClip);

						ImGui::Checkbox("Fixed Aspect Ratio", &cameraComp.FixedAspectRatio);

						if (!cameraComp.FixedAspectRatio)
						{
							ImGui::Text("Aspect Ratio: %.3f", camera.GetAspectRatio());

							float aspectRatio = camera.GetAspectRatio();
							if (ImGui::DragFloat("Aspect Ratio", &aspectRatio, 0.1f))
								camera.SetAspectRatio(aspectRatio);
						}
					}
				});

			DrawComponent<RigidBody2DComponent>(entity, "Rigid Body 2D Component", [](RigidBody2DComponent& component)
				{
					const char* rigidBodyTypes[] = { "Staitc", "Dynamic", "Kinematic" };
					const char* currentRigidBodyTypes = rigidBodyTypes[(int)component.Type];

					if (ImGui::BeginCombo("Rigid Body Type", currentRigidBodyTypes))
					{
						for (int i = 0; i < 3; i++)
						{
							bool selected = rigidBodyTypes[i] == currentRigidBodyTypes;
							if (ImGui::Selectable(rigidBodyTypes[i], &selected))
							{
								currentRigidBodyTypes = rigidBodyTypes[i];
								component.Type = (Tassel::RigidBodyType)i;
							}

							if (selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}

					ImGui::Checkbox("Fix Rotation", &component.FixedRotation);
				});

			DrawComponent<BoxCollider2DComponent>(entity, "Box Collider 2D Component", [](BoxCollider2DComponent& component)
				{
					ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset), 0.1f);
					ImGui::DragFloat2("Size", glm::value_ptr(component.Size), 0.1f);
					ImGui::DragFloat("Density", &component.Density, 0.1f);
					ImGui::DragFloat("Friction", &component.Friction, 0.1f);
					ImGui::DragFloat("Restitution", &component.Restitution, 0.1f);
					ImGui::DragFloat("RestitutionThreshold", &component.RestitutionThreshold, 0.1f);
				});
		}
		ImGui::End();
	}
}
