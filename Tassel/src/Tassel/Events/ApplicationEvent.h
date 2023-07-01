#pragma once
#include "Tassel/Events/Event.h"

namespace Tassel
{
	class TASSEL_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) {}

		inline uint32_t GetWidth() const { return m_Width; }
		inline uint32_t GetHeight() const { return m_Height; }

		virtual std::string ToString() const override
		{
			return "WindowResizeEvent: Width:" + std::to_string(m_Width) + " Height:" + std::to_string(m_Height);
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		uint32_t m_Width, m_Height;
	};


	class TASSEL_API WindowMoveEvent : public Event
	{
	public:
		WindowMoveEvent(float x, float y) : m_XPos(x), m_YPos(y) {}

		inline float GetXPos() const { return m_XPos; }
		inline float GetYPos() const { return m_YPos; }

		virtual std::string ToString() const override
		{
			return "WindowMoveEvent: X:" + std::to_string(m_XPos) + " Y:" + std::to_string(m_YPos);
		}

		EVENT_CLASS_TYPE(WindowMove)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		float m_XPos, m_YPos;
	};


	class TASSEL_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


	class TASSEL_API WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent() {}

		EVENT_CLASS_TYPE(WindowFocus)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


	class TASSEL_API WindowLostFocusEvent : public Event
	{
	public:
		WindowLostFocusEvent() {}

		EVENT_CLASS_TYPE(WindowLostFocus)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


	class TASSEL_API ApplicationTickEvent : public Event
	{
	public:
		ApplicationTickEvent() {}
		
		EVENT_CLASS_TYPE(ApplicationTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


	class TASSEL_API ApplicationUpdateEvent : public Event
	{
	public:
		ApplicationUpdateEvent() {}
		
		EVENT_CLASS_TYPE(ApplicationUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


	class TASSEL_API ApplicationRenderEvent : public Event
	{
	public:
		ApplicationRenderEvent() {}
		
		EVENT_CLASS_TYPE(ApplicationRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}
