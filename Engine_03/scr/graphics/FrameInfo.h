#pragma once


// vulkan headers
#include <vulkan/vulkan.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace zt::graphics
{
	struct GlobalUbo {
		glm::mat4 projectionMatrix{ 1.f };
		glm::mat4 viewMatrix{ 1.f };
		glm::vec4 ambientLightColor{ 1.0f, 1.0f, 1.0f, .02f };
		glm::vec3 lightPosition{ 0.0f,0.0f,-10.0f };
		alignas(16) glm::vec4 lightColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	};

	struct FrameInfo
	{
		int frameIndex;
		float aspectRatio;
		VkCommandBuffer commandBuffer;
		VkDescriptorSet globalDescriptorSet;
		GlobalUbo& ubo;
	};
}
