#include "ModelRendererSystem.h"
#include "../components/Transform.h"
#include "../components/Model.h"
#include "../components/Camera.h"
#include "../graphics/FrameInfo.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <stdexcept>
#include <glm/glm.hpp>


namespace zt::system
{
	struct SimplePushConstantData {
		glm::mat4 modelMatrix{ 1.f };
		glm::mat4 normalMatrix{ 1.f };
	};

	void ModelRendererSystem::init(graphics::Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
	{
		createPipelineLayout(globalSetLayout, device);
		createPipeline(renderPass, device);
	}

	void ModelRendererSystem::update(core::Coordinator& coordinator, 
		const graphics::FrameInfo& frameInfo, 
		const core::Entity& cameraEntity) const
	{
		pipeline->bind(frameInfo.commandBuffer);

		vkCmdBindDescriptorSets(
			frameInfo.commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			pipelineLayout,
			0,
			1,
			&frameInfo.globalDescriptorSet,
			0,
			nullptr
		);

		auto const& camera = coordinator.getComponent<component::Camera>(cameraEntity);

		for(auto& modelEntity : entities)
		{
			auto const& transform = coordinator.getComponent<component::Transform>(modelEntity);
			auto const& model = coordinator.getComponent<component::Model>(modelEntity);

			SimplePushConstantData push{};
			push.modelMatrix = transform.mat4();
			push.normalMatrix = transform.normalMatrix();

			vkCmdPushConstants(
				frameInfo.commandBuffer,
				pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(SimplePushConstantData),
				&push);
			model.model->bind(frameInfo.commandBuffer);
			model.model->draw(frameInfo.commandBuffer);
		}

		frameInfo.ubo.projectionMatrix = camera.projection;
		frameInfo.ubo.viewMatrix = camera.view;
	}

	void ModelRendererSystem::clean(graphics::Device& device)
	{
		vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
	}

	void ModelRendererSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout, graphics::Device& device)
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		const std::vector descriptorSetLayout{ globalSetLayout };

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayout.size());
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayout.data();
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
		if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
			VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout!");
		}
	}

	void ModelRendererSystem::createPipeline(VkRenderPass renderPass, graphics::Device& device)
	{
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		graphics::PipelineConfigInfo pipelineConfig{};
		graphics::Pipeline::defaultPipelineConfig(pipelineConfig);
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;
		pipeline = std::make_unique<graphics::Pipeline>(
			device,
			"shaders/default.vert.spv",
			"shaders/default.frag.spv",
			pipelineConfig);
	}
}
