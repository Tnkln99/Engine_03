#pragma once
#include "GameObject.h"
#include "Camera.h"

namespace Zt
{
	struct FrameInfo
	{
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		Camera& camera;
		VkDescriptorSet globalDescriptorSet;
		GameObject::map& gameObjects;
	};
}
