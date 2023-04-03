#pragma once

#include "Camera.h"
#include "Device.h"
#include "GameObject.h"
#include "Pipeline.h"

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

		void renderGameObjects(VkCommandBuffer commandBuffer,
			std::vector<GameObject>& gameObjects, const Camera& camera);

	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

		Device& lveDevice;

		std::unique_ptr<Pipeline> lvePipeline;
		VkPipelineLayout pipelineLayout;
	};
}

