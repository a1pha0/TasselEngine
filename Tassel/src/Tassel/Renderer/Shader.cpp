#include "Tassel/Core/Base.h"
#include "Tassel/Renderer/Shader.h"
#include "Tassel/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Tassel
{
	Shader* Shader::Create(const char* vertexShaderFilepath, const char* pixelShaderFilepath)
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::None:
			TASSEL_CORE_ASSERT(false, "RenderAPI is None");
			return nullptr;
		case Renderer::API::OpenGL:
			return new OpenGLShader(vertexShaderFilepath, pixelShaderFilepath);
		}

		TASSEL_CORE_ASSERT(false, "Unknown RenderAPI");
		return nullptr;
	}

	ShaderLibrary::~ShaderLibrary()
	{
		for (auto& it = m_ShaderLibrary.begin(); it != m_ShaderLibrary.end(); ++it)
		{
			if (it->second)
			{
				delete it->second;
				it->second = nullptr;
			}
		}
		m_ShaderLibrary.clear();
	}

	void ShaderLibrary::Add(const char* name, Shader* shader)
	{
		if (IsExist(name))
		{
			TASSEL_CORE_ERROR("The '{0}' Shader is already exist!", name);
			return;
		}
		m_ShaderLibrary[name] = shader;
	}

	Shader* ShaderLibrary::Add(const char* name, const char* vertexShaderFilepath, const char* pixelShaderFilepath)
	{
		if (IsExist(name))
		{
			TASSEL_CORE_ERROR("The '{0}' Shader is already exist!", name);
			return nullptr;
		}
		Shader* shader = Shader::Create(vertexShaderFilepath, pixelShaderFilepath);
		m_ShaderLibrary[name] = shader;
		return shader;
	}

	void ShaderLibrary::Remove(const char* name)
	{
		if (!IsExist(name))
		{
			TASSEL_CORE_ERROR("The '{0}' Shader is not exist!", name);
		}
		else
		{
			m_ShaderLibrary.erase(name);
		}
	}

	Shader* ShaderLibrary::Get(const char* name)
	{
		if (!IsExist(name))
		{
			TASSEL_CORE_ERROR("The '{0}' Shader is not exist!", name);
			return nullptr;
		}
		return m_ShaderLibrary[name];
	}


	bool ShaderLibrary::IsExist(const char* name)
	{
		return m_ShaderLibrary.find(name) != m_ShaderLibrary.end();
	}

}
