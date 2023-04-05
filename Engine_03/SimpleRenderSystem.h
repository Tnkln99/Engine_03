#pragma once

#include "Camera.h"
#include "Device.h"
#include "GameObject.h"
#include "Pipeline.h"
#include "FrameInfo.h"

// std
#include <memory>
#include <vector>

namespace Zt
{
	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(Device& device, VkRenderPass renderPass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void renderGameObjects(FrameInfo& frameInfo,
			std::vector<GameObject>& gameObjects);

	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

		Device& device;

		std::unique_ptr<Pipeline> lvePipeline;
		VkPipelineLayout pipelineLayout;
	};
}

