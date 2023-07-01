#pragma once
#include "Tassel/Core/Base.h"

namespace Tassel
{
	class TASSEL_API UUID
	{
	public:
		UUID();
		UUID(const UUID& other) = default;
		UUID(uint64_t uuid) : m_UUID(uuid) {}

		inline uint64_t GetUUID() const { return m_UUID; }

	private:
		uint64_t m_UUID;
	};
}
