#pragma once
#include "Tassel/Core/Base.h"
#include "Tassel/Scene/Scene.h"
#include "Tassel/Scene/Entity.h"

namespace Tassel
{
	class TASSEL_API Script
	{
		friend class Entity;

	public:
		Script() = default;
		virtual ~Script() = default;

		virtual void OnCreate() {}
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnDestory() {}

		Entity m_Entity;
	protected:
	private:
	};
}
