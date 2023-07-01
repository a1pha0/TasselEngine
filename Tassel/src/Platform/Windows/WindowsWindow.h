#pragma once
#include "Tassel/Core/Window.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Tassel
{
	class TASSEL_API WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowDesc& desc);
		virtual ~WindowsWindow();

		virtual void OnUpdate() override;

		virtual inline uint32_t GetWidth() const override { return m_Data.Width; }
		virtual inline uint32_t GetHeight() const override { return m_Data.Height; }

		virtual inline void SetEventCallback(const EventCallbackFunc& callback) override { m_Data.EventCallback = callback; }
		
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync(bool) override { return m_Data.VSync; }

		inline virtual void* GetNativeWindow() const { return m_Window; }
	private:
		virtual void Init(const WindowDesc& properties);
		virtual void ShutDown();

	private:
		struct WindowData
		{
			const char* Title;
			uint32_t Width, Height;
			bool VSync;

			EventCallbackFunc EventCallback;
		};

		WindowData m_Data;

		GLFWwindow* m_Window;
		OpenGLContext* m_GraphicContext;
	};
}
