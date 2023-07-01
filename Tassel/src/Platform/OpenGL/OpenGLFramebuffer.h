#pragma once
#include "Tassel/Renderer/Framebuffer.h"

namespace Tassel
{
	class TASSEL_API OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferDesc& desc);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		void Bind() const override;
		void Unbind() const override;

		inline const FramebufferDesc& GetDesc() const override { return m_Desc; }
		inline uint32_t GetColorAttachmentId(uint32_t index = 0) const override { return m_ColorAttachmentsID[index]; }

		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) const override;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) const override;

		virtual void Resize(uint32_t width, uint32_t height) override;

	private:
		uint32_t m_FramebufferId = 0;
		FramebufferDesc m_Desc;

		std::vector<uint32_t> m_ColorAttachmentsID;
		uint32_t m_DepthAttachmentId = 0;

		std::vector<FBTextureFormatDesc> m_ColorAttachmentsDesc;
		FBTextureFormatDesc m_DepthAttachmentDesc;
	};
}