#pragma once
#include "Buffer.h"
#include "Camera.h"

namespace Zt
{
	struct FrameInfo
	{
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		Camera& camera;
	};
}
