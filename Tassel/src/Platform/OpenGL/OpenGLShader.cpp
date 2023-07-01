#include "Tassel/Core/Base.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Tassel/Core/Log.h"

namespace Tassel
{
	std::string OpenGLShader::ReadShaderFile(const char* filepath)
	{
		std::ifstream ifs(filepath, std::ios::in);
		if (!ifs.is_open())
		{
			TASSEL_CORE_ERROR("Failed to open '{0}'", filepath);
			return std::string();
		}

		std::ostringstream oss;
		oss << ifs.rdbuf();
		ifs.close();
		return oss.str();
	}

	uint32_t OpenGLShader::CompileShader(uint32_t type, const char* source)
	{
		uint32_t shader = glCreateShader(type);
		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);

		int isCompiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			int length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			char* log = new char[length];
			glGetShaderInfoLog(shader, length, &length, log);

			TASSEL_CORE_ERROR("{0}", log);
			delete[] log;
			glDeleteShader(shader);
			TASSEL_CORE_ASSERT(false, "Compile Shader Failed");
			return 0;
		}
		return shader;
	}

	uint32_t OpenGLShader::CreateProgram(uint32_t vertexShader, uint32_t pixelShader)
	{
		uint32_t program = glCreateProgram();

		glAttachShader(program, vertexShader);
		glAttachShader(program, pixelShader);

		glLinkProgram(program);
		glValidateProgram(program);

		int isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			int length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

			char* log = new char[length];
			glGetProgramInfoLog(program, length, &length, log);

			TASSEL_CORE_ERROR("{0}", log);

			glDeleteProgram(program);
			glDeleteShader(vertexShader);
			glDeleteShader(pixelShader);

			TASSEL_CORE_ASSERT(false, "Link Program Failed");
		}

		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, pixelShader);

		glDeleteShader(vertexShader);
		glDeleteShader(pixelShader);
		return program;
	}

	OpenGLShader::OpenGLShader(const char* vertexShaderFilepath, const char* pixelShaderFilepath)
		: m_ShaderId(0)
	{
		const std::string vertexShaderSource = ReadShaderFile(vertexShaderFilepath);
		const std::string pixelShaderSource = ReadShaderFile(pixelShaderFilepath);

		uint32_t vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource.c_str());
		uint32_t pixelShader = CompileShader(GL_FRAGMENT_SHADER, pixelShaderSource.c_str());

		m_ShaderId = CreateProgram(vertexShader, pixelShader);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_ShaderId);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_ShaderId);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetUniformInt(const char* name, int value) const
	{
		int location = glGetUniformLocation(m_ShaderId, name);
		if (location < 0)
			TASSEL_CORE_ERROR("Could't find {0} in shader", name);
		else
			glUniform1i(location, value);
	}

	void OpenGLShader::SetUniformIntArray(const char* name, const int* values, uint32_t count) const
	{
		int location = glGetUniformLocation(m_ShaderId, name);
		if (location < 0)
			TASSEL_CORE_ERROR("Could't find {0} in shader", name);
		else
			glUniform1iv(location, count, values);
	}

	void OpenGLShader::SetUniformVec2(const char* name, const glm::vec2& vector2) const
	{
		int location = glGetUniformLocation(m_ShaderId, name);
		if (location < 0)
			TASSEL_CORE_ERROR("Could't find {0} in shader", name);
		else
			glUniform2f(location, vector2.x, vector2.y);
	}

	void OpenGLShader::SetUniformVec3(const char* name, const glm::vec3& vector3) const
	{
		int location = glGetUniformLocation(m_ShaderId, name);
		if (location < 0)
			TASSEL_CORE_ERROR("Could't find {0} in shader", name);
		else
			glUniform3f(location, vector3.x, vector3.y, vector3.z);
	}

	void OpenGLShader::SetUniformVec4(const char* name, const glm::vec4& vector4) const
	{
		int location = glGetUniformLocation(m_ShaderId, name);
		if (location < 0)
			TASSEL_CORE_ERROR("Could't find {0} in shader", name);
		else
			glUniform4f(location, vector4.x, vector4.y, vector4.z, vector4.w);
	}

	void OpenGLShader::SetUniformMat4(const char* name, const glm::mat4& matrix4) const
	{
		int location = glGetUniformLocation(m_ShaderId, name);
		if (location < 0)
			TASSEL_CORE_ERROR("Could't find {0} in shader", name);
		else
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix4));
	}

	void OpenGLShader::DebugPrintAttriAndUniform()
	{
		GLint i = 0;
		GLint count = 0;

		GLint size = 0; // size of the variable
		GLenum type = 0; // type of the variable (float, vec3 or mat4, etc)

		const GLsizei bufSize = 16; // maximum name length
		GLchar name[bufSize]; // variable name in GLSL
		GLsizei length = 0; // name length

		glGetProgramiv(m_ShaderId, GL_ACTIVE_ATTRIBUTES, &count);
		printf("Active Attributes: %d\n", count);

		for (i = 0; i < count; i++)
		{
			glGetActiveAttrib(m_ShaderId, (GLuint)i, bufSize, &length, &size, &type, name);

			printf("Attribute #%d Type: %u Name: %s\n", i, type, name);
		}

		glGetProgramiv(m_ShaderId, GL_ACTIVE_UNIFORMS, &count);
		printf("Active Uniforms: %d\n", count);

		for (i = 0; i < count; i++)
		{
			glGetActiveUniform(m_ShaderId, (GLuint)i, bufSize, &length, &size, &type, name);

			printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
		}
	}
}
