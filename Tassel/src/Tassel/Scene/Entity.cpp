#include "Tassel/Core/Base.h"
#include "Tassel/Scene/Entity.h"
#include "Tassel/Scene/Component.h"

namespace Tassel
{
	Entity::Entity(entt::entity entityID, Scene* scene)
		: m_EntityID(entityID), m_Scene(scene)
	{

	}

	Tassel::Entity Entity::Copy(Scene* destScene, Entity& srcEntity, bool sameUUID)
	{
		std::string tag = srcEntity.GetComponent<TagComponent>().Tag + " - Copy";

		Entity destEntity;
		if (sameUUID)
		{
			uint64_t uuid = srcEntity.GetComponent<UUIDComponent>().GetUUID();
			destEntity = destScene->CreateEntity(uuid, tag);
		}
		else
			destEntity = destScene->CreateEntity(tag);

		Entity::CopyComponent<TransformComponent>(destEntity, srcEntity);
		Entity::CopyComponent<SpriteRendererComponent>(destEntity, srcEntity);
		Entity::CopyComponent<CircleMeshComponent>(destEntity, srcEntity);
		Entity::CopyComponent<CameraComponent>(destEntity, srcEntity);
		Entity::CopyComponent<RigidBody2DComponent>(destEntity, srcEntity);
		Entity::CopyComponent<BoxCollider2DComponent>(destEntity, srcEntity);

		return destEntity;
	}
}
