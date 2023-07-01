#pragma once
#include "Tassel/Core/Base.h"
#include "Tassel/Core/Log.h"

namespace Tassel
{
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Mat3, Mat4, Bool
	};

	struct BufferLayoutElement
	{
		ShaderDataType Type;
		std::string Name;
		uint32_t Offset;
		bool Normalized;

		BufferLayoutElement(ShaderDataType type, std::string name = "Default", bool normalized = false)
			: Type(type), Name(name), Offset(0), Normalized(normalized) {}
	};

	static GLenum ShaderDataTypeToGLDataType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::None:      return GL_NONE;
		case ShaderDataType::Float:     return GL_FLOAT;
		case ShaderDataType::Float2:    return GL_FLOAT;
		case ShaderDataType::Float3:    return GL_FLOAT;
		case ShaderDataType::Float4:    return GL_FLOAT;
		case ShaderDataType::Int:       return GL_INT;
		case ShaderDataType::Int2:      return GL_INT;
		case ShaderDataType::Int3:      return GL_INT;
		case ShaderDataType::Int4:      return GL_INT;
		case ShaderDataType::Mat3:      return GL_FLOAT;
		case ShaderDataType::Mat4:      return GL_FLOAT;
		case ShaderDataType::Bool:      return GL_BOOL;
		}
		TASSEL_CORE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	static uint32_t ElementCountOfShaderDataType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::None:      return 0;
		case ShaderDataType::Float:     return 1;
		case ShaderDataType::Float2:    return 2;
		case ShaderDataType::Float3:    return 3;
		case ShaderDataType::Float4:    return 4;
		case ShaderDataType::Int:       return 1;
		case ShaderDataType::Int2:      return 2;
		case ShaderDataType::Int3:      return 3;
		case ShaderDataType::Int4:      return 4;
		case ShaderDataType::Mat3:      return 3 * 3;
		case ShaderDataType::Mat4:      return 4 * 4;
		case ShaderDataType::Bool:      return 1;
		}
		TASSEL_CORE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	class TASSEL_API BufferLayout
	{
	public:
		BufferLayout();
		BufferLayout(const std::initializer_list<BufferLayoutElement>& elments);

		inline const std::vector<BufferLayoutElement>& GetElements() { return m_Elements; }
		inline uint32_t GetStride() { return m_Stride; }

	private:
		std::vector<BufferLayoutElement> m_Elements;
		uint32_t m_Stride;
	};

	class TASSEL_API VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}
		
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, size_t size) const = 0;

		virtual void SetBufferLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetBufferLayout() const = 0;

		static VertexBuffer* Create(size_t size);
		static VertexBuffer* Create(const void* data, size_t size);
	};

	class TASSEL_API IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual inline uint32_t GetCount() const = 0;

		static IndexBuffer* Create(const uint32_t* data, uint32_t count);
	};

	class TASSEL_API VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(VertexBuffer* buffer) = 0;
		virtual void SetIndexBuffer(IndexBuffer* buffer) = 0;

		virtual inline const std::vector<VertexBuffer*>& GetVertexBuffer() const = 0;
		virtual inline const IndexBuffer* GetIndexBuffer() const = 0;

		static VertexArray* Create();
	};
}
