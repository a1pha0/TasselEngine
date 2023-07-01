#include "Tassel/Core/Base.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Tassel
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size)
	{
		glGenBuffers(1, &m_BufferId);
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(const void* data, size_t size)
	{
		glGenBuffers(1, &m_BufferId);
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_BufferId);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, size_t size) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* data, uint32_t count)
	{
		glGenBuffers(1, &m_BufferId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, data, GL_STATIC_DRAW);
		m_Count = count;
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_BufferId);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferId);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_VertexArrayId);
		glBindVertexArray(m_VertexArrayId);
		m_VertexBuffers = std::vector<VertexBuffer*>();
		m_IndexBuffer = nullptr;
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		if (!m_VertexBuffers.empty())
		{
			for (VertexBuffer* buffer : m_VertexBuffers)
			{
				delete buffer;
				buffer = nullptr;
			}
		}
		delete m_IndexBuffer;
		m_IndexBuffer = nullptr;
		glDeleteVertexArrays(1, &m_VertexArrayId);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_VertexArrayId);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(VertexBuffer* buffer)
	{
		glBindVertexArray(m_VertexArrayId);

		uint32_t index = 0;
		BufferLayout layout = buffer->GetBufferLayout();
		const std::vector<BufferLayoutElement>& elements = layout.GetElements();

		for (const BufferLayoutElement& element : elements)
		{
			switch (element.Type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, ElementCountOfShaderDataType(element.Type),
					ShaderDataTypeToGLDataType(element.Type),
					element.Normalized, layout.GetStride(),
					(const void*)element.Offset);
				break;
			}
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribIPointer(index, ElementCountOfShaderDataType(element.Type),
					ShaderDataTypeToGLDataType(element.Type),
					layout.GetStride(), (const void*)element.Offset);
				break;
			}
			case ShaderDataType::None:
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
				TASSEL_CORE_ASSERT(false, "Uncompleted shader data type!");
				break;
			}
			index++;
		}
		m_VertexBuffers.push_back(buffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(IndexBuffer* buffer)
	{
		glBindVertexArray(m_VertexArrayId);
		buffer->Bind();
		m_IndexBuffer = buffer;
	}
}
