#pragma once
#include "Tassel/Core/Base.h"

namespace Tassel
{
	class TASSEL_API FileDialog
	{
	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};
}
