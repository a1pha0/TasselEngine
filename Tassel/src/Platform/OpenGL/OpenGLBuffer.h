#pragma once
#include "Tassel/Renderer/Buffer.h"

namespace Tassel
{
	class TASSEL_API OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(size_t size);
		OpenGLVertexBuffer(const void* data, size_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void SetData(const void* data, size_t size) const override;

		virtual void SetBufferLayout(const BufferLayout& layout) override { m_BufferLayout = layout; }
		virtual const BufferLayout& GetBufferLayout() const override { return m_BufferLayout; }

	private:
		uint32_t m_BufferId;
		BufferLayout m_BufferLayout;
	};

	class TASSEL_API OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const uint32_t* data, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const override { return m_Count; }

	private:
		uint32_t m_BufferId;
		uint32_t m_Count;
	};

	class TASSEL_API OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(VertexBuffer* buffer) override;
		virtual void SetIndexBuffer(IndexBuffer* buffer) override;

		virtual inline const std::vector<VertexBuffer*>& GetVertexBuffer() const override { return m_VertexBuffers; }
		virtual inline const IndexBuffer* GetIndexBuffer() const override { return m_IndexBuffer; }
		
	private:
		uint32_t m_VertexArrayId;
		std::vector<VertexBuffer*> m_VertexBuffers;
		IndexBuffer* m_IndexBuffer;
	};
}
