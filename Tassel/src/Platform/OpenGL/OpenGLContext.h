#pragma once
#include "Tassel/Renderer/GraphicContext.h"

struct GLFWwindow;

namespace Tassel
{
	class TASSEL_API OpenGLContext : public GraphicContext
	{
	public:
		OpenGLContext(GLFWwindow* window);

		void Init() const override;
		void SwapBuffers() const override;

	private:
		GLFWwindow* m_Window;
	};
}
