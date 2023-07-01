#include "Tassel/Core/Base.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"
#include "Tassel/Core/Log.h"

namespace Tassel
{
	static void CreateTextures(bool multisample, uint32_t* id, uint32_t count)
	{
		if (multisample)
			glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, count, id);
		else
			glCreateTextures(GL_TEXTURE_2D, count, id);
	}

	static void BindTextures(bool multisample, uint32_t id)
	{
		if (multisample)
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
		else
			glBindTexture(GL_TEXTURE_2D, id);
	}

	static void AttachColorTexture(uint32_t id, uint32_t samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, uint32_t index)
	{
		if (samples > 1)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D_MULTISAMPLE, id, 0);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
			//glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, id, 0);
		}
	}

	static void AttachDepthTexture(uint32_t id, uint32_t samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
	{
		if (samples > 1)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D_MULTISAMPLE, id, 0);
		}
		else
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
			//glTexImage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Desc.Width, m_Desc.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_BYTE, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, id, 0);
		}
	}

	static GLenum TasselFBTextureFormatToGL(FBTextureFormat format)
	{
		switch (format)
		{
		case Tassel::FBTextureFormat::NONE:				return GL_NONE;
		case Tassel::FBTextureFormat::RGBA8:			return GL_RGBA8;
		case Tassel::FBTextureFormat::RED_INTEGER:		return GL_RED_INTEGER;
		case Tassel::FBTextureFormat::DEPTH24S_TENCIL8: return GL_DEPTH24_STENCIL8;
		}
		return GL_NONE;
	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferDesc& desc)
		: m_Desc(desc)
	{
		for (auto& textureFormatDesc : m_Desc.AttachmentDesc.TextureFormats)
		{
			switch (textureFormatDesc.TextureFormat)
			{
			case FBTextureFormat::NONE:
				break;
			case FBTextureFormat::RGBA8:
			case FBTextureFormat::RED_INTEGER:
				m_ColorAttachmentsDesc.emplace_back(textureFormatDesc);
				break;
			case FBTextureFormat::DEPTH24S_TENCIL8:
				m_DepthAttachmentDesc = textureFormatDesc;  // TODO: Why cover the previous one?
				break;
			default:
				break;
			}
		}

		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_FramebufferId);
		glDeleteTextures(m_ColorAttachmentsID.size(), m_ColorAttachmentsID.data());
		glDeleteTextures(1, &m_DepthAttachmentId);
	}

	void OpenGLFramebuffer::Invalidate()
	{
		if (m_FramebufferId)
		{
			glDeleteFramebuffers(1, &m_FramebufferId);
			glDeleteTextures(m_ColorAttachmentsID.size(), m_ColorAttachmentsID.data());
			glDeleteTextures(1, &m_DepthAttachmentId);
			m_ColorAttachmentsID.clear();
			m_DepthAttachmentId = 0;
		}

		glCreateFramebuffers(1, &m_FramebufferId);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferId);

		bool multisample = m_Desc.Samples > 1;

		if (m_ColorAttachmentsDesc.size() > 0)
		{
			m_ColorAttachmentsID.resize(m_ColorAttachmentsDesc.size());
			CreateTextures(multisample, m_ColorAttachmentsID.data(), m_ColorAttachmentsID.size());

			for (size_t i = 0; i < m_ColorAttachmentsID.size(); i++)
			{
				BindTextures(multisample, m_ColorAttachmentsID[i]);
				switch (m_ColorAttachmentsDesc[i].TextureFormat)
				{
				case FBTextureFormat::RGBA8:
					AttachColorTexture(m_ColorAttachmentsID[i], m_Desc.Samples, GL_RGBA8, GL_RGBA, m_Desc.Width, m_Desc.Height, i);
					break;
				case FBTextureFormat::RED_INTEGER:
					AttachColorTexture(m_ColorAttachmentsID[i], m_Desc.Samples, GL_R32I, GL_RED_INTEGER, m_Desc.Width, m_Desc.Height, i);
					break;
				default:
					break;
				}
			}
		}

		if (m_DepthAttachmentDesc.TextureFormat != FBTextureFormat::NONE)
		{
			CreateTextures(multisample, &m_DepthAttachmentId, 1);
			BindTextures(multisample, m_DepthAttachmentId);

			switch (m_DepthAttachmentDesc.TextureFormat)
			{
			case FBTextureFormat::DEPTH24S_TENCIL8:
				AttachDepthTexture(m_DepthAttachmentId, m_Desc.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_ATTACHMENT, m_Desc.Width, m_Desc.Height);
				break;
			default:
				break;
			}

		}

		if (m_ColorAttachmentsDesc.size() > 1)
		{
			TASSEL_CORE_ASSERT(m_ColorAttachmentsDesc.size() <= 4, "");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(4, buffers);
		}
		else if (m_ColorAttachmentsDesc.empty())
		{
			glDrawBuffer(GL_NONE);
		}

		TASSEL_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferId);
	}

	void OpenGLFramebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferId);
		glViewport(0, 0, m_Desc.Width, m_Desc.Height);
	}

	void OpenGLFramebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y) const
	{
		Bind();
		TASSEL_CORE_ASSERT(attachmentIndex < m_ColorAttachmentsID.size(), "The index does not exist!");
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData = -1;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData); // TODO: Try to change the hard-coded
		Unbind();
		return pixelData;
	}

	void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value) const
	{
		TASSEL_CORE_ASSERT(attachmentIndex < m_ColorAttachmentsID.size(), "The index does not exist!");
		glClearTexImage(m_ColorAttachmentsID[attachmentIndex], 0, TasselFBTextureFormatToGL(m_ColorAttachmentsDesc[attachmentIndex].TextureFormat), GL_INT, &value);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		m_Desc.Width = width;
		m_Desc.Height = height;

		Invalidate();
	}
}
