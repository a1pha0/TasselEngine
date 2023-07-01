#pragma once
#include "Tassel/Core/Base.h"

namespace Tassel::Math
{
	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& transltation, glm::vec3& rotation, glm::vec3& scale);
}
