#pragma once
#include "Tassel/Events/Event.h"

namespace Tassel
{
	class TASSEL_API MouseButtonEvent : public Event
	{
	public:
		inline uint32_t GetMouseButton() { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse)
	protected:
		MouseButtonEvent(int button) : m_Button(button) {}

		uint32_t m_Button;
	};


	class TASSEL_API MouseButtonDownEvent : public MouseButtonEvent
	{
	public:
		MouseButtonDownEvent(uint32_t button) : MouseButtonEvent(button) {}

		virtual std::string ToString() const override
		{
			return "MouseButtonDownEvent: " + std::to_string(m_Button);
		}

		EVENT_CLASS_TYPE(MouseButtonDown)
	};


	class TASSEL_API MouseButtonUpEvent : public MouseButtonEvent
	{
	public:
		MouseButtonUpEvent(uint32_t button) : MouseButtonEvent(button) {}

		virtual std::string ToString() const override
		{
			return "MouseButtonUpEvent: " + std::to_string(m_Button);
		}

		EVENT_CLASS_TYPE(MouseButtonUp)
	};


	class TASSEL_API MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(float x, float y) : m_MouseX(x), m_MouseY(y) {}

		inline float GetMouseX() const { return m_MouseX; }
		inline float GetMouseY() const { return m_MouseY; }

		virtual std::string ToString() const override
		{
			return "MouseMoveEvent: X:" + std::to_string(m_MouseX) + " Y:" + std::to_string(m_MouseY);
		}

		EVENT_CLASS_TYPE(MouseMove)
		EVENT_CLASS_CATEGORY(EventCategoryMouse)
	private:
		float m_MouseX, m_MouseY;
	};


	class TASSEL_API MouseWheelEvent : public Event
	{
	public:
		MouseWheelEvent(float xOffset, float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		virtual std::string ToString() const override
		{
			return "MouseWheelEvent: X Offset:" + std::to_string(m_XOffset) + " Y Offset:" + std::to_string(m_YOffset);
		}

		EVENT_CLASS_TYPE(MouseWheel)
		EVENT_CLASS_CATEGORY(EventCategoryMouse)
	private:
		float m_XOffset, m_YOffset;
	};
}
