#include "Tassel/Core/Base.h"
#include "Tassel/Renderer/Texture.h"
#include "Tassel/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Tassel
{
	Tassel::Texture2D* Texture2D::Create(const char* filepath)
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::None:
			TASSEL_CORE_ASSERT(false, "RenderAPI is None");
			return nullptr;
		case Renderer::API::OpenGL:
			return new OpenGLTexture2D(filepath);
		}

		TASSEL_CORE_ASSERT(false, "Unknown RenderAPI");
		return nullptr;
	}

	Tassel::Texture2D* Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::None:
			TASSEL_CORE_ASSERT(false, "RenderAPI is None");
			return nullptr;
		case Renderer::API::OpenGL:
			return new OpenGLTexture2D(width, height);
		}

		TASSEL_CORE_ASSERT(false, "Unknown RenderAPI");
		return nullptr;
	}

	SubTexture2D::SubTexture2D(const Texture2D* texture, glm::vec2 minCoord, glm::vec2 maxCoord)
		: m_Texture(texture)
	{
		m_TexCoord[0] = minCoord;
		m_TexCoord[1] = { maxCoord.x, minCoord.y };
		m_TexCoord[2] = maxCoord;
		m_TexCoord[3] = { minCoord.x, maxCoord.y };

		m_AspectRatio = (maxCoord.x - minCoord.x) / (maxCoord.y - minCoord.y);
	}

	Tassel::SubTexture2D* SubTexture2D::Create(const Texture2D* texture, glm::vec2 leftBottomPixel, glm::vec2 topRightPixel)
	{
		glm::vec2 minCoord = { leftBottomPixel.x / texture->GetWidth(), leftBottomPixel.y / texture->GetHeight() };
		glm::vec2 maxCoord = { topRightPixel.x / texture->GetWidth(), topRightPixel.y / texture->GetHeight() };
		return new Tassel::SubTexture2D(texture, minCoord, maxCoord);
	}

	Tassel::SubTexture2D* SubTexture2D::Create(const Texture2D* texture, glm::vec2 leftBottomCoord, glm::vec2 topRightCoord, glm::vec2 cellSize)
	{
		glm::vec2 minCoord = { (leftBottomCoord.x * cellSize.x) / texture->GetWidth(), (leftBottomCoord.y * cellSize.y) / texture->GetHeight() };
		glm::vec2 maxCoord = { (topRightCoord.x * cellSize.x) / texture->GetWidth(), (topRightCoord.y * cellSize.y) / texture->GetHeight() };
		return new Tassel::SubTexture2D(texture, minCoord, maxCoord);
	}
}
