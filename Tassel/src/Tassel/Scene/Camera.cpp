#include "Tassel/Core/Base.h"
#include "Tassel/Scene/Camera.h"
#include "Tassel/Core/Input.h"

namespace Tassel
{
	void Camera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		RecalculateProjectionMatrix();
	}

	void Camera::SetPerspective(float fov, float nearClip, float farClip)
	{
		m_PerspectiveFOV = fov;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;
		RecalculateProjectionMatrix();
	}

	void Camera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;
		RecalculateProjectionMatrix();
	}

	void Camera::RecalculateProjectionMatrix()
	{
		if (m_ProjectionType == Camera::ProjectionType::Perspective)
		{
			m_ProjectionMatrix = glm::perspective(glm::radians(m_PerspectiveFOV), m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
		else if (m_ProjectionType == Camera::ProjectionType::Orthographic)
		{
			float left = -m_OrthographicSize * m_AspectRatio * 0.5;
			float right = m_OrthographicSize * m_AspectRatio * 0.5;
			float bottom = -m_OrthographicSize * 0.5;
			float top = m_OrthographicSize * 0.5;
			m_ProjectionMatrix = glm::ortho(left, right, bottom, top, m_OrthographicNear, m_OrthographicFar);
		}
	}

	//----------------------------------------------------------------
	//-----Editor Camera----------------------------------------------
	//----------------------------------------------------------------
	static glm::vec2 MousePosition = { 0.0f, 0.0f }; // TODO: Try to use another way to realize this

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
	{
		UpdateProjection();
		UpdateView();
		MousePosition = Input::GetMousePosition();
	}

	void EditorCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::UpdateView()
	{
		glm::quat orientation = GetOrientation();
		m_ViewMatrix = glm::inverse(glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation));
	}

	void EditorCamera::OnUpdate(float deltaTime)
	{
		if (Input::IsMouseButtonDown(TASSEL_MOUSE_BUTTON_RIGHT))
		{
			float speed = 5.0f;
			if (Input::IsKeyDown(TASSEL_KEY_W))
				m_Position += GetForwardVector() * deltaTime * speed;
			if (Input::IsKeyDown(TASSEL_KEY_S))
				m_Position -= GetForwardVector() * deltaTime * speed;

			if (Input::IsKeyDown(TASSEL_KEY_A))
				m_Position -= GetRightVector() * deltaTime * speed;
			if (Input::IsKeyDown(TASSEL_KEY_D))
				m_Position += GetRightVector() * deltaTime * speed;

			if (Input::IsKeyDown(TASSEL_KEY_Q))
				m_Position -= GetUpVector() * deltaTime * speed;
			if (Input::IsKeyDown(TASSEL_KEY_E))
				m_Position += GetUpVector() * deltaTime * speed;

			glm::vec2 deltaPosition = Input::GetMousePosition() - MousePosition;
			m_Pitch += deltaPosition.y * deltaTime * 1.0f;
			m_Yaw += deltaPosition.x * deltaTime * 1.0f;
		}
		UpdateView();
		MousePosition = Input::GetMousePosition();
	}

	void EditorCamera::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseWheelEvent>(TASSEL_BIND_EVENT_FUNC(EditorCamera::OnMouseWheel));
	}

	bool EditorCamera::OnMouseWheel(MouseWheelEvent& event)
	{
		m_Position += GetForwardVector() * event.GetYOffset() * 0.3f;
		UpdateView();
		return false;
	}
}
