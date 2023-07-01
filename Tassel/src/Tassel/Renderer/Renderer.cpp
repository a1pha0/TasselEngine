#include "Tassel/Core/Base.h"
#include "Tassel/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Tassel/Scene/Component.h"

namespace Tassel
{
	Renderer::API Renderer::m_API = Renderer::API::OpenGL;
	RendererAPI* Renderer::m_RendererAPI = new OpenGLRendererAPI;
	Renderer::ScenceData Renderer::m_ScenceData;

	void Renderer::BeginScene(const Camera* camera)
	{
		m_ScenceData.ProjectionMatrix = camera->GetProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Shader* shader, const VertexArray* vertexArray)
	{
		shader->Bind();
		vertexArray->Bind();

		shader->SetUniformMat4("u_ViewProjection", m_ScenceData.ProjectionMatrix);
		shader->SetUniformMat4("u_Transform", glm::mat4(1.0f));

		DrawIndexed(vertexArray);
	}


	//------------------------------------------------------------------------------------
	//--------2D Renderer-----------------------------------------------------------------
	//------------------------------------------------------------------------------------

	Renderer2D::API Renderer2D::m_API = Renderer2D::API::OpenGL;

	RendererAPI* Renderer2D::m_Renderer2DAPI = new OpenGLRendererAPI;

	Renderer2D::Statistic Renderer2D::m_Statistic;

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexSlot;
		int EntityID;
	};

	struct ScenceData2D
	{
		const uint32_t maxRectangleCount = 100;
		const uint32_t maxVectexCount = maxRectangleCount * 4;
		const uint32_t maxIndexCount = maxRectangleCount * 6;

		Vertex* verticesBase = nullptr;
		Vertex* verticesPtr = nullptr;
		uint32_t indicesCounter = 0;

		Texture2D* whiteTexture = nullptr;
		static const uint32_t maxTexSlotsCount = 32;
		std::array<const Texture*, maxTexSlotsCount> textureSlots;
		uint32_t textureSlotIndex = 1;

		glm::mat4 viewProjectionMatrix{0.0f};
		VertexArray* vertexArray = nullptr;
		VertexBuffer* vertexBuffer = nullptr;
		Shader* shader = nullptr;
		ShaderLibrary* shaderLibrary = nullptr;
	};

	static ScenceData2D* s_ScenceData2D = new ScenceData2D;

	void Renderer2D::Init()
	{
		m_Renderer2DAPI->Init();

		s_ScenceData2D->vertexBuffer = VertexBuffer::Create(s_ScenceData2D->maxVectexCount * sizeof(Vertex));
		BufferLayout layout = { {Tassel::ShaderDataType::Float3, "Position"},
								{Tassel::ShaderDataType::Float4, "Color"},
								{Tassel::ShaderDataType::Float2, "TexCoord"},
								{Tassel::ShaderDataType::Float, "TexSlot" },
								{Tassel::ShaderDataType::Int, "EntityID"} };
		s_ScenceData2D->vertexBuffer->SetBufferLayout(layout);

		uint32_t* indices = new uint32_t[s_ScenceData2D->maxIndexCount];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_ScenceData2D->maxIndexCount; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;
			
			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		IndexBuffer* indexBuffer = IndexBuffer::Create(indices, s_ScenceData2D->maxIndexCount);
		s_ScenceData2D->vertexArray = VertexArray::Create();
		s_ScenceData2D->vertexArray->AddVertexBuffer(s_ScenceData2D->vertexBuffer);
		s_ScenceData2D->vertexArray->SetIndexBuffer(indexBuffer);
		delete[] indices;

		// Default Shader
		s_ScenceData2D->shaderLibrary = new ShaderLibrary;
		s_ScenceData2D->shader = Shader::Create("assets/shaders/vertex.shader", "assets/shaders/pixel.shader");
		s_ScenceData2D->shaderLibrary->Add("default", s_ScenceData2D->shader);

		s_ScenceData2D->shader->Bind();
		s_ScenceData2D->shader->SetUniformMat4("u_Transform", glm::mat4{ 1.0f });
		
		int sampler[s_ScenceData2D->maxTexSlotsCount];
		for (uint32_t i = 0; i < s_ScenceData2D->maxTexSlotsCount; i++)
			sampler[i] = i;
		s_ScenceData2D->shader->Bind();
		s_ScenceData2D->shader->SetUniformIntArray("u_Texture", sampler, s_ScenceData2D->maxTexSlotsCount);

		s_ScenceData2D->verticesBase = new Vertex[s_ScenceData2D->maxVectexCount];

		// slot 0 is white texture
		s_ScenceData2D->whiteTexture = Texture2D::Create(1, 1);
		uint32_t white = 0xffffffff;
		s_ScenceData2D->whiteTexture->SetData(&white, sizeof(uint32_t));
		s_ScenceData2D->textureSlots[0] = s_ScenceData2D->whiteTexture;

		ResetStatistic();
	}

	void Renderer2D::ShutDown()
	{
		delete[] s_ScenceData2D->verticesBase;
		s_ScenceData2D->verticesBase = nullptr;

		delete s_ScenceData2D->vertexArray;
		s_ScenceData2D->vertexArray = nullptr;

		delete s_ScenceData2D->shaderLibrary;
		s_ScenceData2D->shaderLibrary = nullptr;

		delete s_ScenceData2D->whiteTexture;
		s_ScenceData2D->whiteTexture = nullptr;

		delete s_ScenceData2D;
		s_ScenceData2D = nullptr;
	}

	void Renderer2D::BeginScene(const Camera* camera, const glm::mat4& transform)
	{
		s_ScenceData2D->viewProjectionMatrix = camera->GetProjectionMatrix() * glm::inverse(transform);

		s_ScenceData2D->shader = s_ScenceData2D->shaderLibrary->Get("default");
		s_ScenceData2D->shader->Bind();
		s_ScenceData2D->shader->SetUniformMat4("u_ViewProjection", s_ScenceData2D->viewProjectionMatrix);

		s_ScenceData2D->vertexArray->Bind();

		s_ScenceData2D->verticesPtr = s_ScenceData2D->verticesBase;
		s_ScenceData2D->indicesCounter = 0;
		s_ScenceData2D->textureSlotIndex = 1;
	}

	void Renderer2D::BeginScene(const EditorCamera* camera)
	{
		s_ScenceData2D->viewProjectionMatrix = camera->GetViewProjection();

		s_ScenceData2D->shader = s_ScenceData2D->shaderLibrary->Get("default");
		s_ScenceData2D->shader->Bind();
		s_ScenceData2D->shader->SetUniformMat4("u_ViewProjection", s_ScenceData2D->viewProjectionMatrix);

		s_ScenceData2D->vertexArray->Bind();

		s_ScenceData2D->verticesPtr = s_ScenceData2D->verticesBase;
		s_ScenceData2D->indicesCounter = 0;
		s_ScenceData2D->textureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		uint32_t dataSize = (unsigned char*)s_ScenceData2D->verticesPtr - (unsigned char*)s_ScenceData2D->verticesBase;
		s_ScenceData2D->vertexBuffer->SetData(s_ScenceData2D->verticesBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		for (uint32_t i = 0; i < s_ScenceData2D->textureSlotIndex; i++)
			s_ScenceData2D->textureSlots[i]->Bind(i);

		DrawIndexed(s_ScenceData2D->vertexArray, s_ScenceData2D->indicesCounter);

		m_Statistic.DrawCallCount++;
	}

	void Renderer2D::DrawCircle(Entity entity)
	{
		if (entity.HasComponent<CircleMeshComponent>())
		{
			TransformComponent& tc = entity.GetComponent<TransformComponent>();
			CircleMeshComponent& cmc = entity.GetComponent<CircleMeshComponent>();
			DrawCircle(cmc.Color, glm::translate(tc.GetTransform(), cmc.Offset), cmc.Edges, cmc.Randius, cmc.Thickness, cmc.Shade, entity.GetEntityID());
		}
	}

	void Renderer2D::DrawCircle(const glm::vec4& color, const glm::mat4& transform, uint32_t edges, float radius, float thickness, float shade, int entityID)
	{
		if (s_ScenceData2D->indicesCounter >= s_ScenceData2D->maxIndexCount)
			EndBatch();

		const uint32_t vectexCount = 4;
		const glm::vec2 texCoords[vectexCount] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		const glm::vec4 vertexPositions[vectexCount] =
		{
			{ -0.5f, -0.5f, 0.0f, 1.0f },
			{  0.5f, -0.5f, 0.0f, 1.0f },
			{  0.5f,  0.5f, 0.0f, 1.0f },
			{ -0.5f,  0.5f, 0.0f, 1.0f }
		};

		for (uint32_t i = 0; i < vectexCount; i++)
		{
			s_ScenceData2D->verticesPtr->Position = transform * vertexPositions[i];
			s_ScenceData2D->verticesPtr->Color = color;
			s_ScenceData2D->verticesPtr->TexCoord = texCoords[i];
			s_ScenceData2D->verticesPtr->TexSlot = 0.0f;
			s_ScenceData2D->verticesPtr->EntityID = entityID;
			s_ScenceData2D->verticesPtr++;
		}

		s_ScenceData2D->indicesCounter += 6;

		m_Statistic.RectCount++;
	}

	void Renderer2D::DrawRectangle(const glm::vec4& color, const glm::vec3& position, const glm::vec2& scale, float rotation, int entityID)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 0.0f });

		DrawRectangle(color, transform, entityID);
	}

	void Renderer2D::DrawRectangle(const Texture2D* texture2d, const glm::vec3& position, const glm::vec2& scale, float rotation, int entityID)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 0.0f });

		DrawRectangle(texture2d, transform, entityID);
	}

	void Renderer2D::DrawRectangle(const SubTexture2D* subTexture2d, const glm::vec3& position, const glm::vec2& scale, float rotation, int entityID)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 0.0f });

		DrawRectangle(subTexture2d, transform, entityID);
	}

	void Renderer2D::DrawRectangle(const glm::vec4& color, const glm::mat4& transform, int entityID)
	{
		if (s_ScenceData2D->indicesCounter >= s_ScenceData2D->maxIndexCount)
			EndBatch();

		const uint32_t vectexCount = 4;
		const glm::vec2 texCoords[vectexCount] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		const glm::vec4 vertexPositions[vectexCount] =
		{
			{ -0.5f, -0.5f, 0.0f, 1.0f },
			{  0.5f, -0.5f, 0.0f, 1.0f },
			{  0.5f,  0.5f, 0.0f, 1.0f },
			{ -0.5f,  0.5f, 0.0f, 1.0f }
		};

		for (uint32_t i = 0; i < vectexCount; i++)
		{
			s_ScenceData2D->verticesPtr->Position = transform * vertexPositions[i];
			s_ScenceData2D->verticesPtr->Color = color;
			s_ScenceData2D->verticesPtr->TexCoord = texCoords[i];
			s_ScenceData2D->verticesPtr->TexSlot = 0.0f;
			s_ScenceData2D->verticesPtr->EntityID = entityID;
			s_ScenceData2D->verticesPtr++;
		}

		s_ScenceData2D->indicesCounter += 6;

		m_Statistic.RectCount++;
	}

	void Renderer2D::DrawRectangle(const Texture2D* texture2d, const glm::mat4& transform, int entityID)
	{
		if (!texture2d)
		{
			TASSEL_ERROR("null pointer");
			return;
		}

		if (s_ScenceData2D->indicesCounter >= s_ScenceData2D->maxIndexCount)
			EndBatch();

		uint32_t textureIndex = 0;
		for (uint32_t i = 1; i < s_ScenceData2D->textureSlotIndex; i++)
		{
			if (s_ScenceData2D->textureSlots[i]->GetId() == texture2d->GetId())
			{
				textureIndex = i;
				break;
			}
		}

		if (textureIndex == 0)
		{
			if (s_ScenceData2D->textureSlotIndex >= s_ScenceData2D->maxTexSlotsCount)
				EndBatch();

			textureIndex = s_ScenceData2D->textureSlotIndex;
			s_ScenceData2D->textureSlots[textureIndex] = texture2d;
			s_ScenceData2D->textureSlotIndex++;
		}

		const glm::vec4 whiteColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		const uint32_t vectexCount = 4;
		const glm::vec2 texCoords[vectexCount] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		const float aspectRatio = (float)texture2d->GetWidth() / (float)texture2d->GetHeight();
		const glm::vec4 vertexPositions[vectexCount] =
		{
			{ -0.5f, -0.5f / aspectRatio, 0.0f, 1.0f },
			{  0.5f, -0.5f / aspectRatio, 0.0f, 1.0f },
			{  0.5f,  0.5f / aspectRatio, 0.0f, 1.0f },
			{ -0.5f,  0.5f / aspectRatio, 0.0f, 1.0f }
		};

		for (uint32_t i = 0; i < vectexCount; i++)
		{
			s_ScenceData2D->verticesPtr->Position = transform * vertexPositions[i];
			s_ScenceData2D->verticesPtr->Color = whiteColor;
			s_ScenceData2D->verticesPtr->TexCoord = texCoords[i];
			s_ScenceData2D->verticesPtr->TexSlot = textureIndex;
			s_ScenceData2D->verticesPtr->EntityID = entityID;
			s_ScenceData2D->verticesPtr++;
		}

		s_ScenceData2D->indicesCounter += 6;

		m_Statistic.RectCount++;
	}

	void Renderer2D::DrawRectangle(const SubTexture2D* subTexture2d, const glm::mat4& transform, int entityID)
	{
		if (!subTexture2d)
		{
			TASSEL_ERROR("null pointer");
			return;
		}

		if (s_ScenceData2D->indicesCounter >= s_ScenceData2D->maxIndexCount)
			EndBatch();

		const Texture* texture = subTexture2d->GetTexture();

		uint32_t textureIndex = 0;
		for (uint32_t i = 1; i < s_ScenceData2D->textureSlotIndex; i++)
		{
			if (s_ScenceData2D->textureSlots[i]->GetId() == texture->GetId())
			{
				textureIndex = i;
				break;
			}
		}

		if (textureIndex == 0)
		{
			if (s_ScenceData2D->textureSlotIndex >= s_ScenceData2D->maxTexSlotsCount)
				EndBatch();

			textureIndex = s_ScenceData2D->textureSlotIndex;
			s_ScenceData2D->textureSlots[textureIndex] = texture;
			s_ScenceData2D->textureSlotIndex++;
		}

		const glm::vec4 whiteColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		const uint32_t vectexCount = 4;
		const glm::vec2* texCoords = subTexture2d->GetTexCoord();

		const float aspectRatio = subTexture2d->GetAspectRatio();
		const glm::vec4 vertexPositions[vectexCount] =
		{
			{ -0.5f, -0.5f / aspectRatio, 0.0f, 1.0f },
			{  0.5f, -0.5f / aspectRatio, 0.0f, 1.0f },
			{  0.5f,  0.5f / aspectRatio, 0.0f, 1.0f },
			{ -0.5f,  0.5f / aspectRatio, 0.0f, 1.0f }
		};

		for (uint32_t i = 0; i < vectexCount; i++)
		{
			s_ScenceData2D->verticesPtr->Position = transform * vertexPositions[i];
			s_ScenceData2D->verticesPtr->Color = whiteColor;
			s_ScenceData2D->verticesPtr->TexCoord = texCoords[i];
			s_ScenceData2D->verticesPtr->TexSlot = textureIndex;
			s_ScenceData2D->verticesPtr->EntityID = entityID;
			s_ScenceData2D->verticesPtr++;
		}

		s_ScenceData2D->indicesCounter += 6;

		m_Statistic.RectCount++;
	}

	void Renderer2D::DrawRectangle(Entity entity)
	{
		TransformComponent& tc = entity.GetComponent<TransformComponent>();
		SpriteRendererComponent& src = entity.GetComponent<SpriteRendererComponent>();
		if (src.Texture)
			DrawRectangle((const Texture2D*)src.Texture, tc.GetTransform(), entity.GetEntityID());
		else
			DrawRectangle(src.Color, tc.GetTransform(), entity.GetEntityID());
	}

	void Renderer2D::EndBatch()
	{
		EndScene();

		s_ScenceData2D->verticesPtr = s_ScenceData2D->verticesBase;
		s_ScenceData2D->indicesCounter = 0;
		s_ScenceData2D->textureSlotIndex = 1;
	}

	void Renderer2D::ResetStatistic()
	{
		m_Statistic.DrawCallCount = 0;
		m_Statistic.RectCount= 0;
	}

}
