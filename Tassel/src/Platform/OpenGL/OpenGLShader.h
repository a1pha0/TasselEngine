#pragma once
#include "Tassel/Renderer/Shader.h"

namespace Tassel
{
	class TASSEL_API OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const char* vertexShaderFilepath, const char* pixelShaderFilepath);

		virtual ~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;

		void SetUniformInt(const char* name, int value) const override;
		void SetUniformIntArray(const char* name, const int* values, uint32_t count) const override;

		void SetUniformVec2(const char* name, const glm::vec2& vector4) const override;
		void SetUniformVec3(const char* name, const glm::vec3& vector3) const override;
		void SetUniformVec4(const char* name, const glm::vec4& vector4) const override;
		void SetUniformMat4(const char* name, const glm::mat4& matrix4) const override;

	private:
		std::string ReadShaderFile(const char* filepath);

		uint32_t CompileShader(uint32_t type, const char* source);

		uint32_t CreateProgram(uint32_t vertexShader, uint32_t pixelShader);

		void DebugPrintAttriAndUniform();

		uint32_t m_ShaderId;
	};
}
