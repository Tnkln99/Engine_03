#pragma once
#include "GameObject.h"
#include "Camera.h"

namespace zt::graphics
{
	struct GlobalUbo {
		glm::mat4 projectionMatrix{ 1.f };
		glm::mat4 viewMatrix{ 1.f };
		glm::vec4 ambientLightColor{ 1.0f, 1.0f, 1.0f, .02f };
		glm::vec3 lightPosition{ -1.0f };
		alignas(16) glm::vec4 lightColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	};

	struct FrameInfo
	{
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		Camera& camera;
		VkDescriptorSet globalDescriptorSet;
		GameObject::Map& gameObjects;
	};
}
