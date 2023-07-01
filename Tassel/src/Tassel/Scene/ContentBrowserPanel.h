#pragma once
#include "Tassel/Core/Base.h"
#include "Tassel/Renderer/Texture.h"

namespace Tassel
{
	class TASSEL_API ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		~ContentBrowserPanel();

		void OnImGuiRender();

	private:
		std::filesystem::path m_CurrentPath = "assets";

		Texture2D* m_DirectoryIcon;
		Texture2D* m_FileIcon;
	};
}
