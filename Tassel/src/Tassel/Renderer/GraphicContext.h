#pragma once
#include "Tassel/Core/Base.h"

namespace Tassel
{
	class TASSEL_API GraphicContext
	{
	public:
		virtual void Init() const = 0;
		virtual void SwapBuffers() const = 0;
	};
}
