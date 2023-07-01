#include "Tassel/Core/Base.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Tassel/Events/ApplicationEvent.h"
#include "Tassel/Events/KeyEvent.h"
#include "Tassel/Events/MouseEvent.h"

namespace Tassel
{
	static bool b_IsGLFWInitialized = false;

	WindowsWindow::WindowsWindow(const WindowDesc& desc)
	{
		Init(desc);
	}

	WindowsWindow::~WindowsWindow()
	{
		glfwDestroyWindow(m_Window);
		delete m_GraphicContext;
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_GraphicContext->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		m_Data.VSync = enabled;
	}

	void WindowsWindow::Init(const WindowDesc& desc)
	{
		m_Data.Title = desc.Title;
		m_Data.Width = desc.Width;
		m_Data.Height = desc.Height;
		TASSEL_CORE_INFO("Creating Window: Title:\"{0}\", width = {1}, height = {2}", m_Data.Title, m_Data.Width, m_Data.Height);

		if (!b_IsGLFWInitialized)
		{
			int status = glfwInit();
			TASSEL_CORE_ASSERT(status, "Failed to initialize glfw");
			glfwSetErrorCallback([](int error_code, const char* description)
				{
					TASSEL_CORE_ERROR("Tassel Error: {0}: {1}", error_code, description);
				});
			b_IsGLFWInitialized = true;
		}

		m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title, nullptr, nullptr);

		m_GraphicContext = new OpenGLContext(m_Window);
		m_GraphicContext->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);

				windowData.Width = width;
				windowData.Height = height;

				WindowResizeEvent event(width, height);
				windowData.EventCallback(event);
			});

		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xpos, int ypos)
			 {
				 WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);

				 WindowMoveEvent event((float)xpos, (float)ypos);
				 windowData.EventCallback(event);
			 });

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
				
				WindowCloseEvent event;
				windowData.EventCallback(event);
			});

		glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused)
			 {
				 WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);

				 if (focused == GLFW_TRUE)
				 {
					 WindowFocusEvent event;
					 windowData.EventCallback(event);
				 }
				 else
				 {
					 WindowLostFocusEvent event;
					 windowData.EventCallback(event);
				 }
			 });

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyDownEvent event(key, 0);
					windowData.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyDownEvent event(key, 1);
					windowData.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyUpEvent event(key);
					windowData.EventCallback(event);
					break;
				}
				default:
					break;
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t codepoint)
			{
				WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyTypedEvent event(codepoint);
				windowData.EventCallback(event);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
				
				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonDownEvent event(button);
					windowData.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonUpEvent event(button);
					windowData.EventCallback(event);
					break;
				}
				default:
					break;
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
				
				MouseWheelEvent event((float)xoffset, (float)yoffset);
				windowData.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
			{
				WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMoveEvent event((float)xpos, (float)ypos);
				windowData.EventCallback(event);
			});
	}

	void WindowsWindow::ShutDown()
	{
		glfwDestroyWindow(m_Window);
	}
}
