#pragma once
#include "Tassel/Core/Base.h"

namespace Tassel
{
	class TASSEL_API Scene;
	class TASSEL_API Entity;

	class TASSEL_API SceneSerializer
	{
	public:
		SceneSerializer() = default;

		void SaveScene(Scene* scene, const std::string& filepath);
		bool LoadScene(Scene* scene, const std::string& filepath);

	private:
		void SerializeEntity(YAML::Emitter& out, Entity& entity);
	};
}
