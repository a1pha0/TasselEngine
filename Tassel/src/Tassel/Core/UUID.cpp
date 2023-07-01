#include "Tassel/Core/Base.h"
#include "Tassel/Core/UUID.h"

namespace Tassel
{
	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformIntDistribution;

	UUID::UUID()
		:m_UUID(s_UniformIntDistribution(s_Engine))
	{
	}
}
