#pragma once
#include "Tassel/Core/Base.h"
#include "Tassel/Scene/Script.h"

namespace Tassel
{
	class TASSEL_API CameraController : public Script
	{
	public:
		CameraController() = default;
		virtual ~CameraController() = default;

		void OnCreate() override;
		void OnUpdate(float deltaTime) override;
		void OnDestory() override;

		inline void SetCameraMoveSpeed(float value) { m_CameraMoveSpeed = value; };
		inline void SetCameraZoomSpeed(float value) { m_CameraZoomSpeed = value; };
		inline void SetCameraRotationSpeed(float value) { m_CameraRotateSpeed = value; };

	private:
		float m_CameraMoveSpeed = 0.001f;
		float m_CameraZoomSpeed = 0.05f;
		float m_CameraRotateSpeed = 360.0f;
	};
}
