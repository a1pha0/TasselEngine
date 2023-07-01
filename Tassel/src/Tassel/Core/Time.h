#pragma once
#include "Tassel/Core/Base.h"

namespace Tassel
{
	class TASSEL_API TimeStep
	{
	public:
		TimeStep(float time = 0.0f) : m_Time(time) {}

		inline float GetSeconds() const { return m_Time; }

	private:
		float m_Time;
	};
}
