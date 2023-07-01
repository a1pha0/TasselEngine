#include "Tassel/Core/Base.h"
#include "Tassel/Scene/SceneSerializer.h"
#include "Tassel/Core/Log.h"
#include "Tassel/Scene/Component.h"

namespace YAML
{
	Emitter& operator<<(Emitter& out, const glm::vec2& vec2)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec2.x << vec2.y << YAML::EndSeq;
		return out;
	}

	Emitter& operator<<(Emitter& out, const glm::vec3& vec3)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec3.x << vec3.y << vec3.z << YAML::EndSeq;
		return out;
	}

	Emitter& operator<<(Emitter& out, const glm::vec4& vec4)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec4.x << vec4.y << vec4.z << vec4.w << YAML::EndSeq;
		return out;
	}

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& vec2)
		{
			Node node;
			node.push_back(vec2.x);
			node.push_back(vec2.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& vec2)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			vec2.x = node[0].as<float>();
			vec2.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& vec3)
		{
			Node node;
			node.push_back(vec3.x);
			node.push_back(vec3.y);
			node.push_back(vec3.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& vec3)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			vec3.x = node[0].as<float>();
			vec3.y = node[1].as<float>();
			vec3.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& vec4)
		{
			Node node;
			node.push_back(vec4.x);
			node.push_back(vec4.y);
			node.push_back(vec4.z);
			node.push_back(vec4.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& vec4)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			vec4.x = node[0].as<float>();
			vec4.y = node[1].as<float>();
			vec4.z = node[2].as<float>();
			vec4.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Tassel
{
	void SceneSerializer::SaveScene(Scene* scene, const std::string& filepath)
	{
		TASSEL_CORE_INFO("Saving Scene: {0}", filepath);

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
	
		out << YAML::Key << "EditorCamera";
		out << YAML::BeginMap;
		out << YAML::Key << "Position" << YAML::Value << scene->m_EditorCamera.GetPosition();
		out << YAML::Key << "Roll" << YAML::Value << scene->m_EditorCamera.GetRoll();
		out << YAML::Key << "Pitch" << YAML::Value << scene->m_EditorCamera.GetPitch();
		out << YAML::Key << "Yaw" << YAML::Value << scene->m_EditorCamera.GetYaw();
		out << YAML::Key << "FOV" << YAML::Value << scene->m_EditorCamera.GetFOV();
		out << YAML::Key << "AspectRatio" << YAML::Value << scene->m_EditorCamera.GetAspectRation();
		out << YAML::Key << "NearClip" << YAML::Value << scene->m_EditorCamera.GetNearClip();
		out << YAML::Key << "FarClip" << YAML::Value << scene->m_EditorCamera.GetFarClip();
		out << YAML::Key << "ViewportWidth" << YAML::Value << scene->m_EditorCamera.GetViewportWidth();
		out << YAML::Key << "ViewportHeight" << YAML::Value << scene->m_EditorCamera.GetViewportHeight();
		out << YAML::EndMap;

		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		auto view = scene->m_Registry.view<TagComponent>();
		for (entt::entity entityID : view)
		{
			Entity entity = { entityID, scene };
			SerializeEntity(out, entity);
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream ofs(filepath, std::fstream::out);
		TASSEL_CORE_ASSERT(ofs.is_open(), "Failed to save the scene!");
		ofs << out.c_str();
	}

	bool SceneSerializer::LoadScene(Scene* scene, const std::string& filepath)
	{
		std::ifstream ifs(filepath);
		TASSEL_CORE_ASSERT(ifs.is_open(), "Failed to open file:", filepath);

		std::stringstream ss;
		ss << ifs.rdbuf();

		YAML::Node sceneData = YAML::Load(ss.str());
		if (!sceneData["Scene"])
		{
			TASSEL_CORE_ERROR("Failed to load scene:", filepath);
			return false;
		}

		std::string name = sceneData["Scene"].as<std::string>();
		TASSEL_CORE_INFO("Loading Scene: {0}", name);

		YAML::Node& editorCameraData = sceneData["EditorCamera"];
		if (editorCameraData)
		{
			scene->m_EditorCamera.SetPosition(editorCameraData["Position"].as<glm::vec3>());
			scene->m_EditorCamera.SetOrientation(editorCameraData["Roll"].as<float>(), editorCameraData["Pitch"].as<float>(), editorCameraData["Yaw"].as<float>());
			scene->m_EditorCamera.SetPerspective(editorCameraData["FOV"].as<float>(), editorCameraData["AspectRatio"].as<float>(), editorCameraData["NearClip"].as<float>(), editorCameraData["FarClip"].as<float>());
			scene->m_EditorCamera.SetViewportSize(editorCameraData["ViewportWidth"].as<float>(), editorCameraData["ViewportHeight"].as<float>());
		}

		YAML::Node& entitiesData = sceneData["Entities"];
		if (entitiesData)
		{
			for (auto entityData : entitiesData)
			{
				uint64_t uuid = entityData["Entity"].as<uint64_t>();

				std::string tag;
				YAML::Node tagCompData = entityData["TagComponent"];
				if (tagCompData)
					name = tagCompData["Tag"].as<std::string>();

				TASSEL_CORE_TRACE("Loading Entity with ID = {0}, name = {1}", uuid, name);

				Entity entity = scene->CreateEntity(uuid, name);
				
				YAML::Node transformCompData = entityData["TransformComponent"];
				if (transformCompData)
				{
					TransformComponent& transformComp = entity.GetComponent<TransformComponent>();
					transformComp.Translation = transformCompData["Translation"].as<glm::vec3>();
					transformComp.Rotation = transformCompData["Rotation"].as<glm::vec3>();
					transformComp.Scale = transformCompData["Scale"].as<glm::vec3>();
				}

				YAML::Node spriteRendererCompData = entityData["SpriteRendererComponent"];
				if (spriteRendererCompData)
				{
					SpriteRendererComponent& spriteRendererComp = entity.AddComponent<SpriteRendererComponent>();
					spriteRendererComp.Color = spriteRendererCompData["Color"].as<glm::vec4>();
				}

				YAML::Node circleMeshComponentData = entityData["CircleMeshComponent"];
				if (circleMeshComponentData)
				{
					CircleMeshComponent& component = entity.AddComponent<CircleMeshComponent>();
					component.Color = circleMeshComponentData["Color"].as<glm::vec4>();
					component.Offset = circleMeshComponentData["Offset"].as<glm::vec3>();

					component.Randius = circleMeshComponentData["Randius"].as<float>();
					component.Edges = circleMeshComponentData["Edges"].as<uint32_t>();
					component.Thickness = circleMeshComponentData["Thickness"].as<float>();
					component.Shade = circleMeshComponentData["Shade"].as<float>();
				}

				YAML::Node cameraCompData = entityData["CameraComponent"];
				if (cameraCompData)
				{
					CameraComponent& cameraComp = entity.AddComponent<CameraComponent>();
					YAML::Node cameraData = cameraCompData["Camera"];
					if (cameraData)
					{
						cameraComp.Camera.SetProjectionType((Camera::ProjectionType)(cameraData["ProjectionType"].as<int>()));
						
						cameraComp.Camera.SetAspectRatio(cameraData["AspectRatio"].as<float>());

						cameraComp.Camera.SetPerspective(cameraData["PerspectiveFOV"].as<float>(), 
														 cameraData["PerspectiveNear"].as<float>(), 
														 cameraData["PerspectiveFar"].as<float>());
						
						cameraComp.Camera.SetOrthographic(cameraData["OrthographicSize"].as<float>(), 
														  cameraData["OrthographicNear"].as<float>(), 
														  cameraData["OrthographicFar"].as<float>());
					}
					cameraComp.Primary = cameraCompData["Primary"].as<bool>();
					cameraComp.FixedAspectRatio = cameraCompData["FixedAspectRatio"].as<bool>();
				}

				YAML::Node rigidBody2DComponentData = entityData["RigidBody2DComponent"];
				if (rigidBody2DComponentData)
				{
					RigidBody2DComponent& component = entity.AddComponent<RigidBody2DComponent>();
					component.Type = (Tassel::RigidBodyType)rigidBody2DComponentData["Type"].as<int>();
					component.FixedRotation = rigidBody2DComponentData["FixedRotation"].as<bool>();
				}

				YAML::Node boxCollider2DComponentData = entityData["BoxCollider2DComponent"];
				if (boxCollider2DComponentData)
				{
					BoxCollider2DComponent& component = entity.AddComponent<BoxCollider2DComponent>();
					component.Offset = boxCollider2DComponentData["Offset"].as<glm::vec2>();
					component.Size = boxCollider2DComponentData["Size"].as<glm::vec2>();
					component.Density = boxCollider2DComponentData["Density"].as<float>();
					component.Friction = boxCollider2DComponentData["Friction"].as<float>();
					component.Restitution = boxCollider2DComponentData["Restitution"].as<float>();
					component.RestitutionThreshold = boxCollider2DComponentData["RestitutionThreshold"].as<float>();
				}
			}
		}

		return true;
	}

	void SceneSerializer::SerializeEntity(YAML::Emitter& out, Entity& entity)
	{
		out << YAML::BeginMap;

		out << YAML::Key << "Entity" << YAML::Value << entity.GetComponent<UUIDComponent>().GetUUID();

		if (entity.HasComponent<TransformComponent>())
		{
			TagComponent& tagComp = entity.GetComponent<TagComponent>();

			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Tag" << YAML::Value << tagComp.Tag;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			TransformComponent& transformComp = entity.GetComponent<TransformComponent>();
			
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Translation" << YAML::Value << transformComp.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << transformComp.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << transformComp.Scale;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			SpriteRendererComponent& spriteRendererComp = entity.GetComponent<SpriteRendererComponent>();

			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComp.Color;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<CircleMeshComponent>())
		{
			CircleMeshComponent& component = entity.GetComponent<CircleMeshComponent>();

			out << YAML::Key << "CircleMeshComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Color" << YAML::Value << component.Color;
			out << YAML::Key << "Offset" << YAML::Value << component.Offset;
			out << YAML::Key << "Randius" << YAML::Value << component.Randius;
			out << YAML::Key << "Edges" << YAML::Value << component.Edges;
			out << YAML::Key << "Thickness" << YAML::Value << component.Thickness;
			out << YAML::Key << "Shade" << YAML::Value << component.Shade;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			CameraComponent& cameraComp = entity.GetComponent<CameraComponent>();

			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;
			
			out << YAML::Key << "Camera";
			out << YAML::BeginMap;
			out << YAML::Key << "ProjectionType" << YAML::Value << cameraComp.Camera.GetProjectionType();
			out << YAML::Key << "AspectRatio" << YAML::Value << cameraComp.Camera.GetAspectRatio();

			out << YAML::Key << "PerspectiveFOV" << YAML::Value << cameraComp.Camera.GetPerspectiveFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << cameraComp.Camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << cameraComp.Camera.GetPerspectiveFarClip();

			out << YAML::Key << "OrthographicSize" << YAML::Value << cameraComp.Camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << cameraComp.Camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << cameraComp.Camera.GetOrthographicFarClip();
			out << YAML::EndMap;
			
			out << YAML::Key << "Primary" << YAML::Value << cameraComp.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComp.FixedAspectRatio;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<RigidBody2DComponent>())
		{
			RigidBody2DComponent& component = entity.GetComponent<RigidBody2DComponent>();

			out << YAML::Key << "RigidBody2DComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Type" << YAML::Value << (int)component.Type;
			out << YAML::Key << "FixedRotation" << YAML::Value << component.FixedRotation;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			BoxCollider2DComponent& component = entity.GetComponent<BoxCollider2DComponent>();

			out << YAML::Key << "BoxCollider2DComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Offset" << YAML::Value << component.Offset;
			out << YAML::Key << "Size" << YAML::Value << component.Size;
			out << YAML::Key << "Density" << YAML::Value << component.Density;
			out << YAML::Key << "Friction" << YAML::Value << component.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << component.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << component.RestitutionThreshold;
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

}