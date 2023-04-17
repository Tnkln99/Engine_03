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

		void update(core::Coordinator& coordinator, GLFWwindow* window, float aspect, float dt);
	private:
		float moveSpeed{ 3.f };
		float lookSpeed{ 1.5f };

		struct KeyMappings
		{
			int moveLeft = GLFW_KEY_A;
			int moveRight = GLFW_KEY_D;
			int moveForward = GLFW_KEY_W;
			int moveBackward = GLFW_KEY_S;
			int moveUp = GLFW_KEY_E;
			int moveDown = GLFW_KEY_Q;
			int lookLeft = GLFW_KEY_LEFT;
			int lookRight = GLFW_KEY_RIGHT;
			int lookUp = GLFW_KEY_UP;
			int lookDown = GLFW_KEY_DOWN;
		}keys;
	};
}


