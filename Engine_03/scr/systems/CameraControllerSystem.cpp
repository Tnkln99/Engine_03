#include "CameraControllerSystem.h"
#include "../components/Camera.h"
#include "../components/Transform.h"

#include <glm/vec3.hpp>
#include <glm/gtc/constants.hpp>


namespace zt::system
{
	void CameraControllerSystem::init()
	{
	}

	void CameraControllerSystem::update(core::Coordinator& coordinator, GLFWwindow* window, float aspect, float dt)
	{
		for(auto &entity : entities)
		{
			auto& camera = coordinator.getComponent<component::Camera>(entity);
			auto& transform = coordinator.getComponent<component::Transform>(entity);

			glm::vec3 rotate{ 0 };
			if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate.y += 1.f;
			if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.f;
			if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate.x += 1.f;
			if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate.x -= 1.f;

			if (dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
				transform.rotation += lookSpeed * dt * normalize(rotate);
			}

			transform.rotation.x = glm::clamp(transform.rotation.x, -1.5f, 1.5f);
			transform.rotation.y = glm::mod(transform.rotation.y, glm::two_pi<float>());

			float yaw = transform.rotation.y;
			const glm::vec3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
			const glm::vec3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
			const glm::vec3 upDir{ 0.f, -1.f, 0.f };

			glm::vec3 moveDir{ 0.f };
			if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
			if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
			if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
			if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
			if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += upDir;
			if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

			if (dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
				transform.translation += moveSpeed * dt * normalize(moveDir);
			}

			camera.setViewYXZ(transform.translation, transform.rotation);
			camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);
		}
	}
}
