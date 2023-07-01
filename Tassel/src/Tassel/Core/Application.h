#pragma once
#include "Tassel/Core/Base.h"
#include "Tassel/Core/Layer.h"
#include "Tassel/Core/Window.h"
#include "Tassel/Events/ApplicationEvent.h"
#include "Tassel/ImGui/ImGuiLayer.h"

namespace Tassel
{
	class TASSEL_API Application
	{
	public:
		Application(const WindowDesc& desc = WindowDesc());
		virtual ~Application();
		
		void Run();
		inline void Close() { m_Running = false; }

		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* overlayer);

		inline ImGuiLayer* GetImGuiLayer() const { return m_ImGuiLayer; }
		inline const Window& GetWindow() const { return *m_Window; }
		static inline Application& Get() { return *s_Instance; }
		
	private:
		bool OnWindowClose(const WindowCloseEvent& event);
		bool OnWindowResize(const WindowResizeEvent& event);

	private:
		static Application* s_Instance;
		
		bool m_Running = true;
		bool m_Minimized = false;

		Window* m_Window;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		float m_LastFrameTime = 0.0f;
	};

	Application* CreateApplication();
}
