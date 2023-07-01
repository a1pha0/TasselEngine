#include "Tassel/Core/Base.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Tassel/Core/Log.h"

namespace Tassel
{
	OpenGLContext::OpenGLContext(GLFWwindow* window)
		: m_Window(window)
	{
	}

	void OpenGLContext::Init() const
	{
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGL(glfwGetProcAddress);
		TASSEL_CORE_ASSERT(status, "Failed to initialize glad");
	}

	void OpenGLContext::SwapBuffers() const
	{
		glfwSwapBuffers(m_Window);
	}
}
