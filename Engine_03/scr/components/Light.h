#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace zt::component
{
	struct Light
	{
		glm::vec3 color{ 1.0f };
	};
}