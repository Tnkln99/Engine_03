#pragma once

#include "ZtDevice.h"
#include "ZtGameObject.h"
#include "ZtPipeline.h"

// std
#include <memory>
#include <vector>

namespace Zt
{
	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(ZtDevice& device, VkRenderPass renderPass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<ZtGameObject>& gameObjects);

	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

		ZtDevice& lveDevice;

		std::unique_ptr<ZtPipeline> lvePipeline;
		VkPipelineLayout pipelineLayout;
	};
}

