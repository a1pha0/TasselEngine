#pragma once
#include "Tassel/Core/Base.h"
#include "Tassel/Core/UUID.h"
#include "Tassel/Scene/Camera.h"
#include "Tassel/Scene/Entity.h"
#include "Tassel/Scene/Script.h"
#include "Tassel/Renderer/Texture.h"

namespace Tassel
{
	struct UUIDComponent
	{
		Tassel::UUID m_UUID;

		UUIDComponent() = default;
		UUIDComponent(const UUIDComponent& other) = default;
		UUIDComponent(const UUID& other) :m_UUID(other) {}
		UUIDComponent(uint64_t uuid) : m_UUID(uuid) {}

		uint64_t GetUUID() const { return m_UUID.GetUUID(); }
	};

	struct TagComponent
	{
		std::string Tag = "Tag";

		TagComponent() = default;
		TagComponent(const TagComponent& other) = default;
		TagComponent(const std::string& tag) : Tag(tag) {}
		TagComponent(const char* tag) : Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale{ 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = default;
		TransformComponent(const glm::vec4& translation) : Translation(translation) {}

		glm::mat4 GetTransform() const 
		{
			return glm::translate(glm::mat4(1.0f), Translation) * glm::toMat4(glm::qua(Rotation)) * glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f };
		Tassel::Texture* Texture = nullptr;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent& other) = default;
		SpriteRendererComponent(const glm::vec4& color, Tassel::Texture* texture) : Color(color), Texture(texture) {}
	};

	struct CircleMeshComponent
	{
		glm::vec4 Color{ 1.0f };
		glm::vec3 Offset{ 0.0f, 0.0f, 0.0f};

		uint32_t Edges = 32;
		float Randius = 1.0f;
		float Thickness = 1.0f;
		float Shade = 0.0f;

		CircleMeshComponent() = default;
		CircleMeshComponent(const CircleMeshComponent&) = default;
	};

	struct CameraComponent
	{
		Tassel::Camera Camera;
		bool Primary = true;
		bool FixedAspectRatio = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent& other) = default;
		CameraComponent(const Tassel::Camera& camera) : Camera(camera) {}
	};

	struct NativeScriptComponent
	{
		Script* Instance = nullptr;

		Script* (*InstantiateScript)() = nullptr;
		void (*DestoryScript)(NativeScriptComponent*) = nullptr;

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return (Script*)(new T()); };
			DestoryScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

	enum class RigidBodyType
	{
		Staitc = 0, Dynamic, Kinematic
	};

	static b2BodyType TasselRigidBodyTypeCastTob2BodyType(Tassel::RigidBodyType type)
	{
		switch (type)
		{
		case RigidBodyType::Staitc: return b2_staticBody;
		case RigidBodyType::Dynamic: return b2_dynamicBody;
		case RigidBodyType::Kinematic: return b2_kinematicBody;
		}
		TASSEL_CORE_ASSERT(false, "Unknown RigidBodyType!");
	}

	struct RigidBody2DComponent
	{
		RigidBodyType Type = RigidBodyType::Staitc;
		bool FixedRotation = false;

		b2Body* Body = nullptr;

		RigidBody2DComponent() = default;
		RigidBody2DComponent(const RigidBody2DComponent&) = default;
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 Offset{ 0.0f, 0.0f };
		glm::vec2 Size{ 0.5f, 0.5f };

		float Density = 1.0f;
		float Friction = 0.2f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 1.0f * b2_lengthUnitsPerMeter;

		b2Fixture* Fixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};
}
