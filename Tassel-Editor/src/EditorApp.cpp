#include <Tassel.h>
#include <Tassel/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Tassel
{
	class TasselEditor : public Application
	{
	public:
		TasselEditor(const WindowDesc& desc = WindowDesc())
			: Application(desc)
		{
			PushLayer(new EditorLayer());
		}

		virtual ~TasselEditor() = default;
	};


	Application* CreateApplication()
	{
		WindowDesc desc = WindowDesc("Tassel Editor", TASSEL_DEFAULT_WINDOW_WIDTH, TASSEL_DEFAULT_WINDOW_HEIGHT);
		return new TasselEditor(desc);
	}
}
