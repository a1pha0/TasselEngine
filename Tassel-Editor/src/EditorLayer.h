#include <Tassel.h>

namespace Tassel
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(float deltaTime) override;
		virtual void OnEvent(Event& event) override;
		virtual void OnImGuiRender() override;

		bool OnKeyDown(KeyDownEvent& event);
		bool OnMouseButtonDown(MouseButtonDownEvent& event);

	private:
		void ToolBarPanel();

		void NewScene();
		void SaveScene();
		void LoadScene();
		void LoadScene(const std::filesystem::path& filepath);
		void SaveSceneAs();

	private:
		Framebuffer* m_Framebuffer = nullptr;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		glm::vec2 m_ViewportPosition{0.0f, 0.0f};
		glm::vec2 m_ViewportSizeWithTabBar{0.0f, 0.0f};

		glm::vec2 m_ViewportSize{ 1.0f, 1.0f };
		bool m_ViewportSizeChanged = false;
		
		enum class SceneState
		{
			Edit,
			Play,
		};
		Scene* m_EditorScene = nullptr;
		Scene* m_RunntimeScene = nullptr;
		SceneState m_SceneState = SceneState::Edit;

		Texture2D* m_PlayIcon = nullptr;
		Texture2D* m_StopIcon = nullptr;

		//Entity m_MouseHoveredEntity = {};

		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;

		SceneSerializer m_SceneSerializer;
		IMGUIZMO_NAMESPACE::OPERATION m_OperationMode = IMGUIZMO_NAMESPACE::TRANSLATE;
	};
}
