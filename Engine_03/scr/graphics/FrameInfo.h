#pragma once
#include "GameObject.h"
#include "Camera.h"

namespace zt::graphics
{
	struct GlobalUbo {
		glm::mat4 projectionMatrix{ 1.f };
		glm::mat4 viewMatrix{ 1.f };
		glm::vec4 ambientLightColor{ 1.0f, 1.0f, 1.0f, .02f };
		glm::vec3 lightPosition{ 0.0f,0.0f,-10.0f };
		alignas(16) glm::vec4 lightColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	};

	struct RenderUpdateInfo
	{
		int frameIndex;
		float aspectRatio;
		VkCommandBuffer commandBuffer;
		VkDescriptorSet globalDescriptorSet;
		GlobalUbo& ubo;
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
