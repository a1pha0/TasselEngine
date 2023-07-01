#pragma once
#include "Tassel/Events/Event.h"

namespace Tassel
{
	struct WindowDesc
	{
		const char* Title;
		uint32_t Width, Height;
		WindowDesc(const char* title = "Tassel Engine",
						 uint32_t width = TASSEL_DEFAULT_WINDOW_WIDTH, uint32_t height = TASSEL_DEFAULT_WINDOW_HEIGHT)
			: Title(title), Width(width), Height(height) {}
	};

	// Interface representing a desktop system based Window
	class TASSEL_API Window
	{
	public:
		using EventCallbackFunc = std::function<void(Event&)>;

		virtual ~Window() {}
		virtual void OnUpdate() = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFunc& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync(bool) = 0;

		inline virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowDesc& desc = WindowDesc());
	};
}
