#pragma once
#include "Tassel/Core/Base.h"
#include "Tassel/Core/Log.h"

namespace Tassel
{
	enum class EventType
	{
		None = 0,
		WindowResize, WindowMove, WindowClose, WindowFocus, WindowLostFocus,
		ApplicationTick, ApplicationUpdate, ApplicationRender,
		KeyDown, KeyUp, KeyTyped,
		MouseButtonDown, MouseButtonUp, MouseWheel, MouseMove
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = TASSEL_BIT(0),
		EventCategoryKeyboard    = TASSEL_BIT(1),
		EventCategoryMouse       = TASSEL_BIT(2)
	};

#define EVENT_CLASS_TYPE(event_type)\
		static EventType GetStaticType() { return EventType::##event_type; }\
		virtual EventType GetEventType() const override { return GetStaticType(); }\
		virtual std::string GetName() const override { return #event_type; }

#define EVENT_CLASS_CATEGORY(event_category) virtual int GetEventCategory() const override { return event_category; }


	class TASSEL_API Event
	{
		friend class EventDispatcher;
	
	public:
		virtual EventType GetEventType() const = 0;
		virtual int GetEventCategory() const = 0;
		virtual std::string GetName() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category) const { return GetEventCategory() & category; }

		friend std::ostream& operator<<(std::ostream& os, const Event& event) { return os << event.ToString(); }

		bool m_Handled = false;
	};


	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event) : m_Event(event) {}

		template<typename T>
		using EventFunc = std::function<bool(T&)>;

		template<typename T>
		bool Dispatch(EventFunc<T> func)
		{
			if (!m_Event.m_Handled && m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};
}
