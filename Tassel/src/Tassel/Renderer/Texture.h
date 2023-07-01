#pragma once
#include "Tassel/Core/Base.h"

namespace Tassel
{
	class TASSEL_API Texture
	{
	public:
		virtual ~Texture() = default;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual void Bind(uint32_t slot) const = 0;

		virtual inline uint32_t GetId() const = 0;
		virtual bool operator==(const Texture* other) const = 0;

		virtual void SetData(const void* data, uint32_t size) const = 0;
	};

	class TASSEL_API Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;

		static Texture2D* Create(const char* filepath);
		static Texture2D* Create(uint32_t width, uint32_t height);
	};

	class TASSEL_API SubTexture2D
	{
	public:
		SubTexture2D(const Texture2D* texture, glm::vec2 minCoord, glm::vec2 maxCoord);
		virtual ~SubTexture2D() = default;

		inline const Texture* GetTexture() const { return m_Texture; }
		inline const glm::vec2* GetTexCoord() const { return m_TexCoord; }
		inline float GetAspectRatio() const { return m_AspectRatio; }

		static SubTexture2D* Create(const Texture2D* texture, glm::vec2 leftBottomPixel, glm::vec2 topRightPixel);
		static SubTexture2D* Create(const Texture2D* texture, glm::vec2 leftBottomCoord, glm::vec2 topRightCoord, glm::vec2 cellSize);

	private:
		const Texture* m_Texture;
		glm::vec2 m_TexCoord[4];
		float m_AspectRatio = 0.0f;
	};
}
