#include "Tassel/Core/Base.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Tassel/Core/Log.h"

namespace Tassel
{
	OpenGLTexture2D::OpenGLTexture2D(const char* filepath)
		: m_TextureId(0), m_Width(0), m_Height(0), m_InternalFormat(0), m_DataFormat(0)
	{
		stbi_set_flip_vertically_on_load(1);

		int width, height, channels;
		stbi_uc* data = stbi_load(filepath, &width, &height, &channels, 4);
		TASSEL_CORE_ASSERT(data, "Failed to load file");
		m_Width = width;
		m_Height = height;

		//if (channels == 4)
		//{
		//	m_InternalFormat = GL_RGBA8;
		//	m_DataFormat = GL_RGBA;
		//}
		//else if (channels == 3)
		//{
		//	m_InternalFormat = GL_RGB8;
		//	m_DataFormat = GL_RGB;
		//}
		//else
		//{
		//	TASSEL_CORE_ASSERT(false, "Unsupported data format");
		//}

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureId);

		glTextureStorage2D(m_TextureId, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_TextureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_TextureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		glTextureParameteri(m_TextureId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_TextureId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_TextureId, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_TextureId(0), m_Width(width), m_Height(height), m_InternalFormat(GL_RGBA8), m_DataFormat(GL_RGBA)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureId);

		glTextureStorage2D(m_TextureId, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_TextureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_TextureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_TextureId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_TextureId, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_TextureId);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_TextureId);
	}

	bool OpenGLTexture2D::operator==(const Texture* other) const
	{
		return this->m_TextureId == ((const OpenGLTexture2D*)other)->m_TextureId;
	}

	void OpenGLTexture2D::SetData(const void* data, uint32_t size) const
	{
		TASSEL_CORE_ASSERT(size == m_Width * m_Height * (m_DataFormat == GL_RGBA ? 4 : 3), "Invaild size");
		glTextureSubImage2D(m_TextureId, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}
}
