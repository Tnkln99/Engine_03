#pragma once
#include "../graphics/Pipeline.h"
#include "../core/System.h"
#include "../core/Coordinator.h"
#include "../graphics/FrameInfo.h"

namespace zt::system
{
	class ModelRendererSystem : public core::System
	{
	public:
		void init(graphics::Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);

		void update(core::Coordinator& coordinator, const graphics::FrameInfo& frameInfo, const core::Entity& camera) const;

		void clean(graphics::Device& device);
	private:
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout, graphics::Device& device);
		void createPipeline(VkRenderPass renderPass, graphics::Device& device);

		std::unique_ptr<graphics::Pipeline> pipeline;
		VkPipelineLayout pipelineLayout{};
	};
}


