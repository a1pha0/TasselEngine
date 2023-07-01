#pragma once
#include "Tassel/Renderer/Texture.h"

namespace Tassel
{
	class TASSEL_API OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const char* filepath);
		OpenGLTexture2D(uint32_t width, uint32_t height);

		virtual ~OpenGLTexture2D();

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }

		void Bind(uint32_t slot) const override;

		virtual inline uint32_t GetId() const override { return m_TextureId; }
		virtual bool operator==(const Texture* other) const override;

		void SetData(const void* data, uint32_t size) const override;

	private:
		uint32_t m_TextureId;
		uint32_t m_Width;
		uint32_t m_Height;

		uint32_t m_InternalFormat;
		uint32_t m_DataFormat;
	};
}
