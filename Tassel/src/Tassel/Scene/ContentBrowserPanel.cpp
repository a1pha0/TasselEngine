#include "Tassel/Core/Base.h"
#include "Tassel/Scene/ContentBrowserPanel.h"
#include "Tassel/Core/Log.h"

namespace Tassel
{
	const static std::filesystem::path s_AssetsPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
	{
		m_DirectoryIcon = Texture2D::Create("assets/icons/folder.png");
		m_FileIcon = Texture2D::Create("assets/icons/files.png");
	}

	ContentBrowserPanel::~ContentBrowserPanel()
	{
		delete m_DirectoryIcon; m_DirectoryIcon = nullptr;
		delete m_FileIcon; m_FileIcon = nullptr;
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content");

		if (m_CurrentPath != s_AssetsPath)
		{
			if (ImGui::Button("<--"))
			{
				m_CurrentPath = m_CurrentPath.parent_path();
			}
		}

		static float padding = 8.0f;
		static float iconSize = 96.0f;
		float panelWidth = ImGui::GetContentRegionAvail().x;
		uint32_t columns = (uint32_t)(panelWidth / (padding + iconSize));
		if (columns < 1)
			columns = 1;

		ImGui::Columns(columns, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentPath))
		{
			const std::filesystem::path& filepath = directoryEntry.path();
			std::string& fullpathString = filepath.string();
			const char* fullpath = fullpathString.c_str();

			Texture2D* icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton(fullpath, (ImTextureID)icon->GetId(), ImVec2(iconSize, iconSize), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::PopStyleColor();

			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", (const void*)fullpath, (strlen(fullpath) + 1) * sizeof(char), ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
				{
					m_CurrentPath /= filepath.filename();
					break;
				}
				// TODO: isn't directory
			}

			ImGui::TextWrapped(filepath.filename().string().c_str());
			ImGui::NextColumn();

		}

		ImGui::End();
	}
}
