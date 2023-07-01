#pragma once
#include "Tassel/Core/Base.h"

namespace Tassel
{
	class TASSEL_API Scene;

	class TASSEL_API Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity entityID, Scene* scene);
		virtual ~Entity() = default;

		template<typename T>
		bool HasComponent()
		{
			return (m_Scene->m_Registry.try_get<T>(m_EntityID) != nullptr);
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityID, std::forward<Args>(args)...);
			m_Scene->OnAddComponent<T>(*this, component);
			return component;
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityID, std::forward<Args>(args)...);
			m_Scene->OnAddComponent<T>(*this, component);
			return component;
		}

		template<typename T>
		void RemoveComponent()
		{
			m_Scene->m_Registry.remove<T>(m_EntityID);
		}

		template<typename T>
		T& GetComponent()
		{
			return m_Scene->m_Registry.get<T>(m_EntityID);
		}

		template<typename T>
		T* TryGetComponent()
		{
			return m_Scene->m_Registry.try_get<T>(m_EntityID);
		}

		template<typename T>
		static void CopyComponent(Entity& dest, Entity& src)
		{
			if (src.HasComponent<T>())
			{
				T component = src.GetComponent<T>();
				dest.AddOrReplaceComponent<T>(component);
			}
		}

		static Entity Copy(Scene* destScene, Entity& srcEntity, bool sameUUID = false);

		inline uint32_t GetEntityID() const { return (uint32_t)m_EntityID; }
		inline entt::entity GetEntityHandle() const { return m_EntityID; }

		//inline bool Vaild() const { return m_Scene->m_Registry.valid(m_EntityID); }
		//operator bool() { return m_Scene->m_Registry.valid(m_EntityID); }
		operator bool() { return m_EntityID != entt::null; }
		
		bool operator==(const Entity& other) { return this->m_EntityID == other.m_EntityID; }
		bool operator!=(const Entity& other) { return this->m_EntityID != other.m_EntityID; }

	private:
		entt::entity m_EntityID = entt::null;
		Scene* m_Scene = nullptr;
	};
}
