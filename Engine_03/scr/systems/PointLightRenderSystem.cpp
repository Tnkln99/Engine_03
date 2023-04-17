#include "PointLightRenderSystem.h"
#include "../components/Light.h"
#include "../components/Transform.h"

#include <stdexcept>

namespace zt::system
{
	void PointLightRenderSystem::init(graphics::Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
	{
		createPipelineLayout(globalSetLayout, device);
		createPipeline(renderPass, device);
	}

	void PointLightRenderSystem::update(core::Coordinator& coordinator,
		const graphics::RenderUpdateInfo& renderUpdateInfo) const
	{
		pipeline->bind(renderUpdateInfo.commandBuffer);

		vkCmdBindDescriptorSets(
			renderUpdateInfo.commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			pipelineLayout,
			0,
			1,
			&renderUpdateInfo.globalDescriptorSet,
			0,
			nullptr
		);

		for(auto &entity : entities)
		{
			auto const& light = coordinator.getComponent<component::Light>(entity);
			if (light.showSource == false) { continue; }
			auto const& transform = coordinator.getComponent<component::Transform>(entity);

			renderUpdateInfo.ubo.lightColor = light.color;
			renderUpdateInfo.ubo.lightPosition = transform.translation;

			vkCmdDraw(renderUpdateInfo.commandBuffer, 6, 1, 0, 0);
		}
	}

	void PointLightRenderSystem::clean(graphics::Device& device) const
	{
		vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
	}

	void PointLightRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout, graphics::Device& device)
	{
		std::vector<VkDescriptorSetLayout> descriptorSetLayout{ globalSetLayout };

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayout.size());
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayout.data();
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;
		if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
			VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout!");
		}
	}

	void PointLightRenderSystem::createPipeline(VkRenderPass renderPass, graphics::Device& device)
	{
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		graphics::PipelineConfigInfo pipelineConfig{};

		graphics::Pipeline::defaultPipelineConfig(pipelineConfig);
		// because default values are configured to work for simple renders system and default shaders.
		pipelineConfig.attributeDescriptions.clear();
		pipelineConfig.bindingDescriptions.clear();

		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;
		pipeline = std::make_unique<graphics::Pipeline>(
			device,
			"shaders/pointLight.vert.spv",
			"shaders/pointLight.frag.spv",
			pipelineConfig);
	}
}
