#include "Tassel/Core/Base.h"
#include "Tassel/Core/Input.h"
#include "Tassel/Scene/Scene.h"
#include "Tassel/Renderer/Renderer.h"
#include "Tassel/Scene/Entity.h"
#include "Tassel/Scene/Component.h"

namespace Tassel
{
	Tassel::Scene* Scene::Copy(Scene* srcScene)
	{
		Scene* destScene = new Scene;
		destScene->m_ViewportSize = srcScene->m_ViewportSize;

		auto view = srcScene->m_Registry.view<UUIDComponent>();
		for (auto entityID : view)
		{
			Entity srcEntity = { entityID, srcScene };
			Entity::Copy(destScene, srcEntity, true);
		}
		return destScene;
	}

	Scene::~Scene()
	{
		delete m_World; m_World = nullptr;
	}

	Entity Scene::CreateEntity(const std::string& tag)
	{
		Entity entity{ m_Registry.create(), this };
		entity.AddComponent<UUIDComponent>();
		entity.AddComponent<TagComponent>(tag);
		entity.AddComponent<TransformComponent>();
		return entity;
	}

	Tassel::Entity Scene::CreateEntity(uint64_t uuid, const std::string& tag)
	{
		Entity entity{ m_Registry.create(), this };
		entity.AddComponent<UUIDComponent>(uuid);
		entity.AddComponent<TagComponent>(tag);
		entity.AddComponent<TransformComponent>();
		return entity;
	}

	void Scene::DestoryEntity(Entity& entity)
	{
		m_Registry.destroy(entity.GetEntityHandle());
	}

	void Scene::UpdateRuntime(float deltaTime)
	{
		{
			auto view = m_Registry.view<NativeScriptComponent>();
			for (entt::entity entityID: view)
			{
				NativeScriptComponent& nsc = m_Registry.get<NativeScriptComponent>(entityID);
				
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = { entityID, this };
					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(deltaTime);
			}
		}

		{
			const int32 velocityIterations = 6;
			const int32 positionIterations = 2;
			m_World->Step(deltaTime, velocityIterations, positionIterations);

			auto view = m_Registry.view<TransformComponent, RigidBody2DComponent>();
			for (auto [entityID, tc, rbc] : view.each())
			{
				tc.Translation.x = rbc.Body->GetPosition().x;
				tc.Translation.y = rbc.Body->GetPosition().y;
				tc.Rotation.z = rbc.Body->GetAngle();
			}
		}

		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto [entityID, transformComp, cameraComp] : view.each())
			{
				if (cameraComp.Primary)
				{
					Renderer2D::BeginScene(&cameraComp.Camera, transformComp.GetTransform());
					{
						auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
						for (auto [entityID, transformComp, spriteComp] : view.each())
						{
							Renderer2D::DrawRectangle(Entity(entityID, this));
						}
					}

					{
						auto view = m_Registry.view<TransformComponent, CircleMeshComponent>();
						for (auto [entityID, tc, cmc] : view.each())
						{
							Renderer2D::DrawCircle(Entity(entityID, this));
						}
					}
					Renderer2D::EndScene();
					break;
				}
			}
		}
	}

	void Scene::UpdateEditor(float deltaTime)
	{
		Renderer2D::BeginScene(&m_EditorCamera);
		{
			auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
			for (auto [entityID, transformComp, spriteComp] : view.each())
			{
				Renderer2D::DrawRectangle(Entity(entityID, this));
			}
		}

		{
			auto view = m_Registry.view<TransformComponent, CircleMeshComponent>();
			for (auto [entityID, tc, cmc] : view.each())
			{
				Renderer2D::DrawCircle(Entity(entityID, this));
			}
		}
		Renderer2D::EndScene();
	}

	void Scene::OnViewportReszie(uint32_t width, uint32_t height)
	{
		m_ViewportSize = { width, height };
		m_EditorCamera.SetViewportSize(width, height);

		auto view = m_Registry.view<CameraComponent>();
		for (auto [entityID, cameraComp] : view.each())
		{
			if (cameraComp.Camera.GetProjectionType() == Camera::Perspective)
			{
				cameraComp.Camera.SetViewportSize(width, height);
			}
			else if (cameraComp.Camera.GetProjectionType() == Camera::Orthographic && cameraComp.FixedAspectRatio)
			{
				cameraComp.Camera.SetViewportSize(width, height);
			}
		}
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto [entityID, cameraComp] : view.each())
		{
			if (cameraComp.Primary)
				return { entityID, this };
		}
		return {};
	}

	void Scene::OnScenePlay()
	{
		m_World = new b2World({ 0.0f, -9.8f });
		
		auto view = m_Registry.view<TransformComponent, RigidBody2DComponent>();
		for (auto entityID : view)
		{
			Entity entity = { entityID, this };
			auto& tc = entity.GetComponent<TransformComponent>();
			auto& rbc = entity.GetComponent<RigidBody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = TasselRigidBodyTypeCastTob2BodyType(rbc.Type);
			bodyDef.position.Set(tc.Translation.x, tc.Translation.y);
			bodyDef.angle = tc.Rotation.z;
			bodyDef.fixedRotation = rbc.FixedRotation;

			rbc.Body = m_World->CreateBody(&bodyDef);

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bcc = entity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape shape;
				shape.SetAsBox(bcc.Size.x * tc.Scale.x, bcc.Size.y * tc.Scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &shape;
				fixtureDef.density = bcc.Density;
				fixtureDef.friction = bcc.Friction;
				fixtureDef.restitution = bcc.Restitution;
				fixtureDef.restitutionThreshold = bcc.RestitutionThreshold;

				bcc.Fixture = rbc.Body->CreateFixture(&fixtureDef);
			}
		}
	}

	void Scene::OnSceneStop()
	{
		delete m_World; m_World = nullptr;
	}

	template<typename T>
	void Scene::OnAddComponent(Entity& entity, T& component)
	{

	}

	template<>
	void Scene::OnAddComponent<CameraComponent>(Entity& entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
	}

	template<>
	void Scene::OnAddComponent<TagComponent>(Entity& entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnAddComponent<TransformComponent>(Entity& entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnAddComponent<SpriteRendererComponent>(Entity& entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnAddComponent<CircleMeshComponent>(Entity& entity, CircleMeshComponent& component)
	{
	}
	
	template<>
	void Scene::OnAddComponent<NativeScriptComponent>(Entity& entity, NativeScriptComponent& component)
	{
	}

	template<>
	void Scene::OnAddComponent<UUIDComponent>(Entity& entity, UUIDComponent& component)
	{
	}

	template<>
	void Scene::OnAddComponent<RigidBody2DComponent>(Entity& entity, RigidBody2DComponent& component)
	{
	}

	template<>
	void Scene::OnAddComponent<BoxCollider2DComponent>(Entity& entity, BoxCollider2DComponent& component)
	{
	}
}
