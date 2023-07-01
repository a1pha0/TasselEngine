#pragma once
#include "Tassel/Core/Base.h"
#include "Tassel/Scene/Scene.h"
#include "Tassel/Scene/Entity.h"

namespace Tassel
{
	class TASSEL_API SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(Scene* scene);
		~SceneHierarchyPanel();

		void SetScene(Scene* scene);

		void OnImGuiRender();

	protected:
		void DrawEntityNode(Entity& entity);
		void DrawDetails(Entity& entity);

	private:
		Scene* m_Scene = nullptr;
	};
}
