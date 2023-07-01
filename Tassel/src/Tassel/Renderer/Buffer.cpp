#include "Tassel/Core/Base.h"
#include "Tassel/Renderer/Buffer.h"
#include "Tassel/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Tassel
{
	Tassel::VertexBuffer* VertexBuffer::Create(size_t size)
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::None:
			TASSEL_CORE_ASSERT(false, "RenderAPI is None");
			return nullptr;
		case Renderer::API::OpenGL:
			return new OpenGLVertexBuffer(size);
		}

		TASSEL_CORE_ASSERT(false, "Unknown RenderAPI");
		return nullptr;
	}

	VertexBuffer* VertexBuffer::Create(const void* data, size_t size)
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::None:
			TASSEL_CORE_ASSERT(false, "RenderAPI is None");
			return nullptr;
		case Renderer::API::OpenGL:
			return new OpenGLVertexBuffer(data, size);
		}

		TASSEL_CORE_ASSERT(false, "Unknown RenderAPI");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(const uint32_t* data, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::None:
			TASSEL_CORE_ASSERT(false, "RenderAPI is None");
			return nullptr;
		case Renderer::API::OpenGL:
			return new OpenGLIndexBuffer(data, count);
		}

		TASSEL_CORE_ASSERT(false, "Unknown RenderAPI");
		return nullptr;
	}

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::None:
			TASSEL_CORE_ASSERT(false, "RenderAPI is None");
			return nullptr;
		case Renderer::API::OpenGL:
			return new OpenGLVertexArray();
		}

		TASSEL_CORE_ASSERT(false, "Unknown RenderAPI");
		return nullptr;
	}


	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::None:      return 0;
		case ShaderDataType::Float:     return 4;
		case ShaderDataType::Float2:    return 4 * 2;
		case ShaderDataType::Float3:    return 4 * 3;
		case ShaderDataType::Float4:    return 4 * 4;
		case ShaderDataType::Int:       return 4;
		case ShaderDataType::Int2:      return 4 * 2;
		case ShaderDataType::Int3:      return 4 * 3;
		case ShaderDataType::Int4:      return 4 * 4;
		case ShaderDataType::Mat3:      return 4 * 3 * 3;
		case ShaderDataType::Mat4:      return 4 * 4 * 4;
		case ShaderDataType::Bool:      return 1;
		}
		TASSEL_CORE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	BufferLayout::BufferLayout()
		: m_Elements(), m_Stride(0)
	{
	}

	BufferLayout::BufferLayout(const std::initializer_list<BufferLayoutElement>& elements)
		: m_Elements(elements), m_Stride(0)
	{
		uint32_t offset = 0, size = 0;
		for (BufferLayoutElement& element : m_Elements)
		{
			size = ShaderDataTypeSize(element.Type);
			element.Offset = offset;
			offset += size;
			m_Stride += size;
		}
	}
}
