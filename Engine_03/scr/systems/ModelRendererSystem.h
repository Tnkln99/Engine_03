#pragma once
#include "../graphics/GraphicsEngine.h"
#include "../graphics/Pipeline.h"
#include "../core/System.h"
#include "../core/Coordinator.h"


namespace zt::system
{
	class ModelRendererSystem : core::System
	{
	public:
		explicit ModelRendererSystem(const graphics::RenderInitInfo& renderInitInfo);

		void update(core::Coordinator& coordinator, const graphics::RenderUpdateInfo& renderUpdateInfo, const core::Entity& camera) const;
	private:
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);

		graphics::Device& device;

		std::unique_ptr<graphics::Pipeline> pipeline;
		VkPipelineLayout pipelineLayout{};
	};
}


