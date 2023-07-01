#pragma once
#include "Tassel/Core/Base.h"

namespace Tassel
{
	class TASSEL_API Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetUniformInt(const char* name, int value) const = 0;
		virtual void SetUniformIntArray(const char* name, const int* values, unsigned int count) const = 0;

		virtual void SetUniformVec2(const char* name, const glm::vec2& vector2) const = 0;
		virtual void SetUniformVec3(const char* name, const glm::vec3& vector3) const = 0;
		virtual void SetUniformVec4(const char* name, const glm::vec4& vector4) const = 0;
		virtual void SetUniformMat4(const char* name, const glm::mat4& matrix4) const = 0;

		static Shader* Create(const char* vertexShaderFilepath, const char* pixelShaderFilepath);
	};

	class TASSEL_API ShaderLibrary
	{
	public:
		virtual ~ShaderLibrary();

		void Add(const char* name, Shader* shader);
		Shader* Add(const char* name, const char* vertexShaderFilepath, const char* pixelShaderFilepath);
		void Remove(const char* name);

		Shader* Get(const char* name);

		bool IsExist(const char* name);

	private:
		std::unordered_map<const char*, Shader*> m_ShaderLibrary;
	};
}
