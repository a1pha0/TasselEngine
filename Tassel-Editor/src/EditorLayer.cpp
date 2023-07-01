#include "EditorLayer.h"

namespace Tassel
{
	EditorLayer::EditorLayer() : Layer("EditorLayer")
	{
	}

	EditorLayer::~EditorLayer()
	{
		delete m_PlayIcon; m_PlayIcon = nullptr;
		delete m_StopIcon; m_StopIcon = nullptr;

		delete m_Framebuffer; m_Framebuffer = nullptr;
		delete m_EditorScene; m_EditorScene = nullptr;
	}

	void EditorLayer::OnAttach()
	{
		FramebufferDesc desc;
		desc.AttachmentDesc = { FBTextureFormat::RGBA8, FBTextureFormat::RED_INTEGER, FBTextureFormat::DEPTH24S_TENCIL8 };
		m_Framebuffer = Framebuffer::Create(desc);

		m_EditorScene = new Scene;
		m_SceneHierarchyPanel.SetScene(m_EditorScene);

		m_EditorScene->Clear();
		m_EditorScene->m_CurrentFilepath = "assets/scenes/scene.tassel";
		m_SceneSerializer.LoadScene(m_EditorScene, m_EditorScene->m_CurrentFilepath);

		m_PlayIcon = Texture2D::Create("assets/icons/play.png");
		m_StopIcon = Texture2D::Create("assets/icons/stop.png");

		IMGUIZMO_NAMESPACE::Style& style = IMGUIZMO_NAMESPACE::GetStyle();
		style.TranslationLineThickness = 5.0f;
		style.RotationLineThickness = 4.0f;
		style.RotationOuterLineThickness = 4.0f;
		style.ScaleLineThickness = 5.0f;
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(float deltaTime)
	{
		if (m_ViewportSizeChanged)
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorScene->OnViewportReszie((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		m_Framebuffer->Bind();
		Renderer2D::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		Renderer2D::Clear();
		Renderer2D::ResetStatistic();
		
		m_Framebuffer->ClearAttachment(1, -1);
		
		if (m_SceneState == SceneState::Edit)
		{
			m_EditorScene->UpdateEditor(deltaTime);
			if (m_ViewportHovered)
				m_EditorScene->m_EditorCamera.OnUpdate(deltaTime);
		}
		else if (m_SceneState == SceneState::Play)
			m_RunntimeScene->UpdateRuntime(deltaTime);

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyDownEvent>(TASSEL_BIND_EVENT_FUNC(EditorLayer::OnKeyDown));
		dispatcher.Dispatch<MouseButtonDownEvent>(TASSEL_BIND_EVENT_FUNC(EditorLayer::OnMouseButtonDown));

		if (m_SceneState == SceneState::Edit && m_ViewportFocused)
			m_EditorScene->m_EditorCamera.OnEvent(event);
	}

	void EditorLayer::ToolBarPanel()
	{
		ImGui::Begin("##ToolBar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

		static float iconSize = 24.0f;
		ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - iconSize) * 0.5f);

		if (m_SceneState == SceneState::Edit)
		{
			if (ImGui::ImageButton((ImTextureID)m_PlayIcon->GetId(), ImVec2(iconSize, iconSize)))
			{
				m_SceneState = SceneState::Play;
				m_RunntimeScene = Scene::Copy(m_EditorScene);
				m_RunntimeScene->OnScenePlay();
			}
		}
		else if (m_SceneState == SceneState::Play)
		{
			if (ImGui::ImageButton((ImTextureID)m_StopIcon->GetId(), ImVec2(iconSize, iconSize)))
			{
				m_SceneState = SceneState::Edit;
				m_RunntimeScene->OnSceneStop();
				m_RunntimeScene = nullptr;
			}
		}
		ImGui::End();
	}

	void EditorLayer::OnImGuiRender()
	{
		static bool opt_open = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		// -------------------------------------------------------------------
		ImGui::Begin("DockSpace", &opt_open, window_flags);

		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N")) NewScene();
				if (ImGui::MenuItem("Open", "Ctrl+O")) LoadScene();
				if (ImGui::MenuItem("Save", "Ctrl+S")) SaveScene();
				if (ImGui::MenuItem("Save As", "Ctrl+Shift+S")) SaveSceneAs();
				if (ImGui::MenuItem("Exit"))
				{
					opt_open = false;
					Application::Get().Close();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::PopStyleVar(2);
		ImGui::End();

		// ----Basic info-------------------------------------------------------
		ImGui::Begin("TasselEditor");
		const Renderer2D::Statistic& statistic = Renderer2D::GetStatistic();
		ImGui::Text("Draw Call Count: %d", statistic.DrawCallCount);
		ImGui::Text("Rectangle Count: %d", statistic.RectCount);
		ImGui::Text("Viewport Focused: %d", m_ViewportFocused);
		ImGui::Text("Viewport Hovered: %d", m_ViewportHovered);

		glm::vec3 position = m_EditorScene->m_EditorCamera.GetPosition();
		glm::vec3 forward = m_EditorScene->m_EditorCamera.GetForwardVector();
		glm::vec3 right = m_EditorScene->m_EditorCamera.GetRightVector();
		glm::vec3 up = m_EditorScene->m_EditorCamera.GetUpVector();

		ImGui::Text("Position: %f, %f, %f", position.x, position.y, position.z);
		ImGui::Text("forward: %f, %f, %f", forward.x, forward.y, forward.z);
		ImGui::Text("right: %f, %f, %f", right.x, right.y, right.z);
		ImGui::Text("up: %f, %f, %f", up.x, up.y, up.z);

		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::Text("%d vertices, %d indices (%d triangles)", io.MetricsRenderVertices, io.MetricsRenderIndices, io.MetricsRenderIndices / 3);
		ImGui::Text("%d visible windows, %d active allocations", io.MetricsRenderWindows, io.MetricsActiveAllocations);

		ImGui::Text("viewport position: x = %d, y = %d", (int)m_ViewportPosition.x, (int)m_ViewportPosition.y);
		ImGui::Text("viewport size with tab bar: x = %d, y = %d", (int)m_ViewportSizeWithTabBar.x, (int)m_ViewportSizeWithTabBar.y);
		ImGui::Text("viewport size: x = %d, y = %d", (int)m_ViewportSize.x, (int)m_ViewportSize.y);

		ImGui::End();

		//--------Viewport------------------------------------------------------
		ImGui::Begin("Viewport");
		ImVec2 viewportPos = ImGui::GetCursorPos();

		Application::Get().GetImGuiLayer()->HandleEvents(!(m_ViewportFocused && m_ViewportHovered));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImVec2 viewportSize = ImGui::GetContentRegionAvail(); // Exclude tab bar

		m_ViewportSizeChanged = false;
		if ((m_ViewportSize.x != viewportSize.x) || (m_ViewportSize.y != viewportSize.y))
		{
			if (viewportSize.x > 0 && viewportSize.y > 0)
			{
				m_ViewportSize.x = viewportSize.x;
				m_ViewportSize.y = viewportSize.y;
				m_ViewportSizeChanged = true;
			}
		}
		uint32_t id = m_Framebuffer->GetColorAttachmentId(0);
		ImGui::Image((void*)id, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::PopStyleVar();

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				std::string file = (const char*)payload->Data;
				std::filesystem::path filepath = file;
				LoadScene(filepath);
			}
			ImGui::EndDragDropTarget();
		}

		ImVec2 windowPos = ImGui::GetWindowPos();
		m_ViewportPosition = { windowPos.x + viewportPos.x, windowPos.y + viewportPos.y };

		ImVec2 viewSize = ImGui::GetWindowSize(); // Include tab bar
		m_ViewportSizeWithTabBar = { viewSize.x, viewSize.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		auto selectedEntity = m_EditorScene->GetSelectedEntity();
		if (m_SceneState == SceneState::Edit && selectedEntity && m_OperationMode > 0)
		{
			IMGUIZMO_NAMESPACE::SetOrthographic(false);
			IMGUIZMO_NAMESPACE::SetDrawlist(nullptr);
			IMGUIZMO_NAMESPACE::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

			const glm::mat4& cameraView = m_EditorScene->m_EditorCamera.GetViewMatrix();
			const glm::mat4& cameraProjection = m_EditorScene->m_EditorCamera.GetProjectionMatrix();

			TransformComponent& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			bool snapping = Input::IsKeyDown(TASSEL_KEY_LEFT_CONTROL);
			float snap = 0.5f;
			if (m_OperationMode == IMGUIZMO_NAMESPACE::ROTATE)
				snap = 5.0f;

			float snaps[] = { snap, snap, snap };
			IMGUIZMO_NAMESPACE::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), m_OperationMode, IMGUIZMO_NAMESPACE::WORLD, glm::value_ptr(transform), nullptr, snapping ? snaps : nullptr);

			if (IMGUIZMO_NAMESPACE::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);
				tc.Translation = translation;
				tc.Rotation += (rotation - tc.Rotation); // Difference ?
				tc.Scale = scale;
			}
		}

		ImGui::End();

		// -------------------------------------------------------------------
		m_SceneHierarchyPanel.OnImGuiRender();
		m_ContentBrowserPanel.OnImGuiRender();
		ToolBarPanel();
	}

	bool EditorLayer::OnKeyDown(KeyDownEvent& event)
	{
		if (event.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyDown(TASSEL_KEY_LEFT_CONTROL) || Input::IsKeyDown(TASSEL_KEY_RIGHT_CONTROL);
		bool shift = Input::IsKeyDown(TASSEL_KEY_LEFT_SHIFT) || Input::IsKeyDown(TASSEL_KEY_RIGHT_SHIFT);
		bool alt = Input::IsKeyDown(TASSEL_KEY_LEFT_ALT) || Input::IsKeyDown(TASSEL_KEY_RIGHT_ALT);
		bool rightMouse = Input::IsMouseButtonDown(TASSEL_MOUSE_BUTTON_RIGHT);

		if (rightMouse)
			return false;

		switch (event.GetKeyCode())
		{
		case TASSEL_KEY_N:
			if (control && !shift && !alt && m_SceneState == SceneState::Edit)
			{
				NewScene();
				return true;
			}
			break;
		case TASSEL_KEY_O:
			if (control && !shift && !alt && m_SceneState == SceneState::Edit)
			{
				LoadScene();
				return true;
			}
			break;
		case TASSEL_KEY_W:
			if (m_ViewportFocused && !control && !shift && !alt && m_SceneState == SceneState::Edit)
			{
				m_OperationMode = IMGUIZMO_NAMESPACE::TRANSLATE;
				return true;
			}
			break;
		case TASSEL_KEY_S:
			if (control && !alt && m_SceneState == SceneState::Edit)
			{
				if (shift)
					SaveSceneAs();
				else
					SaveScene();
				return true;
			}
			break;
		case TASSEL_KEY_E:
			if (m_ViewportFocused && !control && !shift && !alt && m_SceneState == SceneState::Edit)
			{
				m_OperationMode = IMGUIZMO_NAMESPACE::SCALE;
				return true;
			}
			break;
		case TASSEL_KEY_R:
			if (m_ViewportFocused && !control && !shift && !alt && m_SceneState == SceneState::Edit)
			{
				m_OperationMode = IMGUIZMO_NAMESPACE::ROTATE;
				return true;
			}
			break;
		case TASSEL_KEY_D:
			if (control && !shift && !alt && m_SceneState == SceneState::Edit && m_EditorScene->m_SelectedEntity)
			{
				m_EditorScene->m_SelectedEntity = Entity::Copy(m_EditorScene, m_EditorScene->m_SelectedEntity);
			}
			break;
		case TASSEL_KEY_DELETE:
			if (m_SceneState == SceneState::Edit && m_EditorScene->m_SelectedEntity)
			{
				m_EditorScene->DestoryEntity(m_EditorScene->m_SelectedEntity);
				m_EditorScene->m_SelectedEntity = {};
			}
		}
		return false;
	}

	bool EditorLayer::OnMouseButtonDown(MouseButtonDownEvent& event)
	{
		if (m_ViewportHovered && event.GetMouseButton() == TASSEL_MOUSE_BUTTON_LEFT && !IMGUIZMO_NAMESPACE::IsOver() && m_SceneState == SceneState::Edit)
		{
			ImVec2 mousePos = ImGui::GetMousePos();
			int mouseX = mousePos.x - m_ViewportPosition.x;
			int mouseY = mousePos.y - m_ViewportPosition.y;
			if (mouseX > -1 && mouseX < (int)m_ViewportSize.x && mouseY > -1 && mouseY < (int)m_ViewportSize.y)
			{
				int pixelData = m_Framebuffer->ReadPixel(1, mouseX, (int)m_ViewportSize.y - mouseY);
				m_EditorScene->m_SelectedEntity = (pixelData < 0) ? Entity{} : Entity{ (entt::entity)pixelData, m_EditorScene };
				return true;
			}
		}
		return false;
	}

	void EditorLayer::NewScene()
	{
		std::string filepath = FileDialog::SaveFile("Tassel Scene (*.tassel)\0*.tassel\0");
		if (!filepath.empty())
		{
			m_EditorScene->Clear();
			m_EditorScene->m_CurrentFilepath = filepath;
			m_SceneSerializer.SaveScene(m_EditorScene, filepath);
		}
	}

	void EditorLayer::SaveScene()
	{
		if (m_EditorScene->m_CurrentFilepath.empty())
		{
			std::string filepath = FileDialog::SaveFile("Tassel Scene (*.tassel)\0*.tassel\0");
			if (!filepath.empty())
			{
				m_EditorScene->m_CurrentFilepath = filepath;
				m_SceneSerializer.SaveScene(m_EditorScene, filepath);
			}
		}
		else
		{
			m_SceneSerializer.SaveScene(m_EditorScene, m_EditorScene->m_CurrentFilepath);
		}
	}

	void EditorLayer::LoadScene()
	{
		std::string filepath = FileDialog::OpenFile("Tassel Scene (*.tassel)\0*.tassel\0");
		LoadScene(filepath);
	}

	void EditorLayer::LoadScene(const std::filesystem::path& filepath)
	{
		if (!filepath.empty() && m_EditorScene->m_CurrentFilepath != filepath.string())
		{
			m_EditorScene->Clear();
			m_EditorScene->m_CurrentFilepath = filepath.string();
			m_SceneSerializer.LoadScene(m_EditorScene, m_EditorScene->m_CurrentFilepath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialog::SaveFile("Tassel Scene (*.tassel)\0*.tassel\0");
		if (!filepath.empty())
		{
			m_SceneSerializer.SaveScene(m_EditorScene, filepath);
		}
	}
}
