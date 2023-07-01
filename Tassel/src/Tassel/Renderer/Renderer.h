#pragma once
#include "Tassel/Core/Base.h"
#include "Tassel/Renderer/Buffer.h"
#include "Tassel/Renderer/Shader.h"
#include "Tassel/Renderer/Texture.h"
#include "Tassel/Scene/Camera.h"
#include "Tassel/Controller/CameraController.h"

namespace Tassel
{
	class RendererAPI
	{
	public:
		virtual void Init() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(const VertexArray* vertexArray, size_t indexCount = 0) = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)= 0;
	};

	//------------------------------------------------------------------------------------
	//--------3D Renderer-----------------------------------------------------------------
	//------------------------------------------------------------------------------------

	class TASSEL_API Renderer
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};
	public:
		inline static void Init() { m_RendererAPI->Init(); }
		
		static void BeginScene(const Camera* camera);

		static void Submit(const Shader* shader, const VertexArray* vertexArray);
		static void EndScene();

		inline static API GetAPI() { return m_API; }

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { m_RendererAPI->SetViewport(x, y, width, height); }
		inline static void SetClearColor(const glm::vec4& color) { m_RendererAPI->SetClearColor(color); }

		inline static void DrawIndexed(const VertexArray* vertexArray, size_t indexCount = 0) { m_RendererAPI->DrawIndexed(vertexArray, indexCount); }
		inline static void Clear() { m_RendererAPI->Clear(); }

	private:
		struct ScenceData
		{
			glm::mat4 ProjectionMatrix;
		};

		static RendererAPI* m_RendererAPI;
		static API m_API;
		static ScenceData m_ScenceData;
	};

	//------------------------------------------------------------------------------------
	//--------2D Renderer-----------------------------------------------------------------
	//------------------------------------------------------------------------------------

	class TASSEL_API Renderer2D
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};

		struct Statistic
		{
			uint32_t DrawCallCount = 0;
			uint32_t RectCount = 0;
		};

	public:
		static void Init();
		static void ShutDown();

		static void BeginScene(const Camera* camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera* camera);

		static void EndScene();
		static void Flush();
		static void EndBatch();

		static void DrawCircle(Entity entity);
		static void DrawCircle(const glm::vec4& color, const glm::mat4& transform, uint32_t edges, float radius, float thickness, float shade, int entityID = -1);

		static void DrawRectangle(Entity entity);
		static void DrawRectangle(const glm::vec4& color, const glm::mat4& transform, int entityID = -1);
		static void DrawRectangle(const Texture2D* texture2d, const glm::mat4& transform, int entityID = -1);
		static void DrawRectangle(const SubTexture2D* subTexture2d, const glm::mat4& transform, int entityID = -1);
		static void DrawRectangle(const glm::vec4& color, const glm::vec3& position, const glm::vec2& scale, float rotation, int entityID = -1);
		static void DrawRectangle(const Texture2D* texture2d, const glm::vec3& position, const glm::vec2& scale, float rotation, int entityID = -1);
		static void DrawRectangle(const SubTexture2D* subTexture2d, const glm::vec3& position, const glm::vec2& scale, float rotation, int entityID = -1);

		inline static API GetAPI() { return m_API; }
		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { m_Renderer2DAPI->SetViewport(x, y, width, height); }
		inline static void SetClearColor(const glm::vec4& color) { m_Renderer2DAPI->SetClearColor(color); }
		inline static void DrawIndexed(const VertexArray* vertexArray, size_t indexCount = 0) { m_Renderer2DAPI->DrawIndexed(vertexArray, indexCount); }
		inline static void Clear() { m_Renderer2DAPI->Clear(); }

		static void ResetStatistic();
		inline static const Statistic& GetStatistic() { return m_Statistic; }

	private:
		static Statistic m_Statistic;
		static RendererAPI* m_Renderer2DAPI;
		static API m_API;
	};
}
