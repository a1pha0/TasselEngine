#pragma once
#include "Tassel/Core/Base.h"

namespace Tassel
{
	enum class FBTextureFormat
	{
		NONE,

		RGBA8,
		RED_INTEGER,

		DEPTH24S_TENCIL8,

	};

	struct FBTextureFormatDesc
	{
		FBTextureFormatDesc() = default;
		FBTextureFormatDesc(FBTextureFormat format) :TextureFormat(format) {}

		FBTextureFormat TextureFormat = FBTextureFormat::NONE;
		// TODO: filtering/wrap
	};

	struct FBAttachmnetDesc
	{
		FBAttachmnetDesc() = default;
		FBAttachmnetDesc(const std::initializer_list<FBTextureFormatDesc>& textureFormats) :TextureFormats(textureFormats) {}

		std::vector<FBTextureFormatDesc> TextureFormats;
	};

	struct FramebufferDesc
	{
		uint32_t Width = TASSEL_DEFAULT_WINDOW_WIDTH, Height = TASSEL_DEFAULT_WINDOW_HEIGHT;
		uint32_t Samples = 1;
		FBAttachmnetDesc AttachmentDesc;
		bool SwapChainTarget = false;
	};

	class TASSEL_API Framebuffer
	{
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) const = 0;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) const = 0;

		virtual inline const FramebufferDesc& GetDesc() const = 0;
		virtual inline uint32_t GetColorAttachmentId(uint32_t index = 0) const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		static Framebuffer* Create(const FramebufferDesc& desc);
	};
}
