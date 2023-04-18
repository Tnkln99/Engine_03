#pragma once
#include "../core/System.h"
#include "../core/Coordinator.h"


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>


namespace zt::system
{
	class CameraControllerSystem : public core::System
	{
	public:
		void init();

		void update(core::Coordinator& coordinator, float aspect, float dt);
	private:
		float moveSpeed{ 3.f };
		float lookSpeed{ 1.5f };
	};
}


