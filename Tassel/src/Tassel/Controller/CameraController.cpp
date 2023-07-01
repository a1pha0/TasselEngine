#include "Tassel/Core/Base.h"
#include "Tassel/Controller/CameraController.h"
#include "Tassel/Core/Input.h"
#include "Tassel/Scene/Component.h"

namespace Tassel
{
	void CameraController::OnCreate()
	{

	}

	void CameraController::OnUpdate(float deltaTime)
	{
		glm::vec3& translation = m_Entity.GetComponent<TransformComponent>().Translation;

		if (Input::IsKeyDown(TASSEL_KEY_W))
			translation[1] += m_CameraMoveSpeed;
		if (Input::IsKeyDown(TASSEL_KEY_S))
			translation[1] -= m_CameraMoveSpeed;
		if (Input::IsKeyDown(TASSEL_KEY_A))
			translation[0] -= m_CameraMoveSpeed;
		if (Input::IsKeyDown(TASSEL_KEY_D))
			translation[0] += m_CameraMoveSpeed;
	}

	void CameraController::OnDestory()
	{

	}
}
