#pragma once
#include "Tassel/Events/Event.h"

namespace Tassel
{
	class TASSEL_API KeyEvent : public Event
	{
	public:
		inline uint32_t GetKeyCode() { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard)
	protected:
		KeyEvent(uint32_t keycode) : m_KeyCode(keycode) {}

		uint32_t m_KeyCode;
	};


	class TASSEL_API KeyDownEvent : public KeyEvent
	{
	public:
		KeyDownEvent(uint32_t keycode, uint32_t repeatCount) : KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		virtual std::string ToString() const override 
		{
			return "KeyDownEvent: " + std::to_string(m_KeyCode) + " (Repeat Counts:" + std::to_string(m_RepeatCount) + ")";
		}

		inline uint32_t GetRepeatCount() const { return m_RepeatCount; }

		EVENT_CLASS_TYPE(KeyDown)
	private:
		uint32_t m_RepeatCount;
	};


	class TASSEL_API KeyUpEvent : public KeyEvent
	{
	public:
		KeyUpEvent(uint32_t keycode) : KeyEvent(keycode) {}

		virtual std::string ToString() const override
		{
			return "KeyUpEvent: " + std::to_string(m_KeyCode);
		}

		EVENT_CLASS_TYPE(KeyUp)
	};


	class TASSEL_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(uint32_t keycode) : KeyEvent(keycode) {}

		virtual std::string ToString() const override
		{
			return "KeyTypedEvent: " + std::to_string(m_KeyCode);
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}
