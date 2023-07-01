#pragma once
#include "Tassel/Renderer/Renderer.h"

namespace Tassel
{
	class TASSEL_API OpenGLRendererAPI : public RendererAPI
	{
	public:
		void Init() override
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_DEPTH_TEST);
		}

		void SetClearColor(const glm::vec4& color) override
		{
			glClearColor(color.r, color.g, color.b, color.a);
		}

		void Clear() override
		{
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		}

		void DrawIndexed(const VertexArray* vertexArray, size_t indexCount) override
		{
			//size_t count = indexCount == 0 ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
		}

		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override
		{
			glViewport(x, y, width, height);
		}
	};
}