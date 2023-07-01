#include "Tassel/Core/Base.h"
#include "Tassel/Renderer/Framebuffer.h"
#include "Tassel/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Tassel
{
	Tassel::Framebuffer* Framebuffer::Create(const FramebufferDesc& desc)
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::None:
			TASSEL_CORE_ASSERT(false, "RenderAPI is None");
			return nullptr;
		case Renderer::API::OpenGL:
			return new OpenGLFramebuffer(desc);
		}

		TASSEL_CORE_ASSERT(false, "Unknown RenderAPI");
		return nullptr;
	}
}