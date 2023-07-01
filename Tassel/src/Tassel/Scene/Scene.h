#pragma once
#include "Tassel/Core/Base.h"
#include "Tassel/Scene/Entity.h"
#include "Tassel/Scene/Camera.h"

namespace Tassel
{
	class TASSEL_API Scene
	{
		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;

	public:
		Scene() = default;
		virtual ~Scene();

		static Scene* Copy(Scene* srcScene);

		Entity CreateEntity(const std::string& tag = "Entity");
		Entity CreateEntity(uint64_t uuid, const std::string& tag = "Entity");

		void DestoryEntity(Entity& entity);

		void UpdateRuntime(float deltaTime);
		void UpdateEditor(float deltaTime);

		void OnScenePlay();
		void OnSceneStop();

		void OnViewportReszie(uint32_t width, uint32_t height);

		template<typename T>
		void OnAddComponent(Entity& entity, T& component);

		void Clear() { m_Registry.clear(); m_SelectedEntity = {}; }

		Entity GetPrimaryCameraEntity();
		Entity GetSelectedEntity() { return m_SelectedEntity; }

		inline EditorCamera& GetEditorCamera() { return m_EditorCamera; }

		std::string m_CurrentFilepath;
		EditorCamera m_EditorCamera;
		glm::vec2 m_ViewportSize{ TASSEL_DEFAULT_WINDOW_WIDTH, TASSEL_DEFAULT_WINDOW_HEIGHT };
		Entity m_SelectedEntity = {};

	private:
		entt::registry m_Registry;

		b2World* m_World = nullptr;
	};
}
