#include "Tassel/Core/Base.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Tassel
{
	Tassel::Window* Window::Create(const WindowDesc& desc)
	{
		return new WindowsWindow(desc);
	}
}
