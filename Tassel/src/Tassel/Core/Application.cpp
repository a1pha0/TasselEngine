#include "Tassel/Core/Base.h"
#include "Tassel/Core/Application.h"
#include "Tassel/Renderer/Renderer.h"

namespace Tassel
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const WindowDesc& desc)
	{
		TASSEL_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Window::Create(desc);
		m_Window->SetEventCallback(TASSEL_BIND_EVENT_FUNC(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushLayer(m_ImGuiLayer);

		Renderer::Init();
		Renderer2D::Init();
	}

	Application::~Application()
	{
		Renderer2D::ShutDown();

		delete m_Window;
		m_Window = nullptr;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = (float)glfwGetTime();

			if (!m_Minimized)
			{
				for (auto it = m_LayerStack.Begin(); it != m_LayerStack.End(); ++it)
				{
					(*it)->OnUpdate(time - m_LastFrameTime);
				}
			}

			m_LastFrameTime = (float)glfwGetTime();
			
			// TODO: Will be realized in another thread
			m_ImGuiLayer->Begin();
			for (auto it = m_LayerStack.Begin(); it != m_LayerStack.End(); ++it)
			{
				(*it)->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(TASSEL_BIND_EVENT_FUNC(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(TASSEL_BIND_EVENT_FUNC(Application::OnWindowResize));

		for (auto it = m_LayerStack.End(); it != m_LayerStack.Begin(); )
		{
			if (event.m_Handled)
				break;
			(*--it)->OnEvent(event);
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverLayer(Layer* overlayer)
	{
		m_LayerStack.PushOverLayer(overlayer);
	}

	bool Application::OnWindowClose(const WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(const WindowResizeEvent& event)
	{
		if (event.GetWidth() == 0 || event.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::SetViewport(0, 0, event.GetWidth(), event.GetHeight());
		return false;
	}
}
