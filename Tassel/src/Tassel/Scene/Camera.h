#pragma once
#include "Tassel/Core/Base.h"
#include "Tassel/Events/Event.h"
#include "Tassel/Events/MouseEvent.h"

namespace Tassel
{
	class TASSEL_API Camera
	{
	public:
		enum ProjectionType
		{
			Perspective = 0, Orthographic = 1
		};

	public:
		Camera() { RecalculateProjectionMatrix(); }
		virtual ~Camera() = default;
		void SetViewportSize(uint32_t width, uint32_t height);

		void SetPerspective(float fov, float nearClip, float farClip);
		inline float GetPerspectiveFOV() const { return m_PerspectiveFOV; }
		inline float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		inline float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
		inline void SetPerspectiveFOV(float fov) { m_PerspectiveFOV = fov; RecalculateProjectionMatrix(); }
		inline void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjectionMatrix(); }
		inline void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; RecalculateProjectionMatrix(); }

		void SetOrthographic(float size, float nearClip, float farClip);
		inline float GetOrthographicSize() const { return m_OrthographicSize; }
		inline float GetOrthographicNearClip() const { return m_OrthographicNear; }
		inline float GetOrthographicFarClip() const { return m_OrthographicFar; }
		inline void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjectionMatrix(); }
		inline void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjectionMatrix(); }
		inline void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; RecalculateProjectionMatrix(); }

		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline void SetProjectionMatrix(const glm::mat4& projection) { m_ProjectionMatrix = projection; }

		inline ProjectionType GetProjectionType() const { return m_ProjectionType; }
		inline void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjectionMatrix(); }

		inline float GetAspectRatio() const { return m_AspectRatio; }
		inline void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; RecalculateProjectionMatrix(); }

	protected:
		void RecalculateProjectionMatrix();

	private:
		float m_PerspectiveFOV = 45.0f;
		float m_PerspectiveNear = 0.001f;
		float m_PerspectiveFar = 1000.0f;

		float m_OrthographicSize = 1.0f;
		float m_OrthographicNear = -1.0f;
		float m_OrthographicFar = 1.0f;

		float m_AspectRatio = 1.0f;
		ProjectionType m_ProjectionType = ProjectionType::Perspective;
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
	};


	class EditorCamera
	{
	public:
		EditorCamera(float fov = 45.0f, float aspectRatio = 1.778f, float nearClip = 0.1f, float farClip = 1000.0f);

		void OnUpdate(float deltaTime);
		void OnEvent(Event& event);

		inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }
		inline void SetPerspective(float fov, float aspectRatio, float nearClip, float farClip)
		{
			m_FOV = fov; m_AspectRatio = aspectRatio; m_NearClip = nearClip; m_FarClip = farClip; UpdateProjection();
		}
		inline void SetPosition(const glm::vec3& position) { m_Position = position; UpdateView(); }
		inline void SetOrientation(float roll, float pitch, float yaw) { m_Roll = roll; m_Pitch = pitch;  m_Yaw = yaw; UpdateView(); }

		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& GetProjectionMatrix() const { return m_Projection; }
		inline glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

		inline const glm::vec3& GetPosition() const { return m_Position; }
		inline glm::vec3 GetUpVector()      const { return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f)); }
		inline glm::vec3 GetRightVector()   const { return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f)); }
		inline glm::vec3 GetForwardVector() const { return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f)); }
		inline glm::quat GetOrientation()   const { return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, -m_Roll)); }

		inline float GetRoll() const { return m_Roll; }
		inline float GetPitch() const { return m_Pitch; }
		inline float GetYaw() const { return m_Yaw; }

		inline float GetFOV() const { return m_FOV; }
		inline float GetAspectRation() const { return m_AspectRatio; }
		inline float GetNearClip() const { return m_NearClip; }
		inline float GetFarClip() const { return m_FarClip; }

		inline float GetViewportWidth() const { return m_ViewportWidth; }
		inline float GetViewportHeight() const { return m_ViewportHeight; }

	private:
		void UpdateProjection();
		void UpdateView();
		bool OnMouseWheel(MouseWheelEvent& event);

	private:
		float m_FOV, m_AspectRatio, m_NearClip, m_FarClip;
		float m_Pitch = 0.0f, m_Yaw = 0.0f, m_Roll = 0.0f;
		glm::vec3 m_Position = { 0.0f, 0.0f, 10.0f };
		float m_ViewportWidth = 1280, m_ViewportHeight = 720;

		glm::mat4 m_ViewMatrix, m_Projection;
	};
}
