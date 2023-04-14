#pragma once
#include "Descriptors.h"
#include "Renderer.h"
#include "Buffer.h"

namespace zt::graphics
{
	struct RenderInitInfo
	{
		Device& device;
		VkRenderPass renderPass;
		VkDescriptorSetLayout globalSetLayout;
		std::vector<VkDescriptorSet> globalDescriptorSets;
		std::vector<std::unique_ptr<Buffer>>& uboBuffers;
	};

	struct RenderUpdateInfo
	{
		int frameIndex;
		float aspectRatio;
		VkCommandBuffer commandBuffer;
		VkDescriptorSet globalDescriptorSet;
		std::vector<std::unique_ptr<Buffer>>& uboBuffers;
	};

	class GraphicsEngine
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		GraphicsEngine();
		~GraphicsEngine();

		GraphicsEngine(const GraphicsEngine&) = delete;
		GraphicsEngine& operator=(const GraphicsEngine&) = delete;

		RenderInitInfo init();
		RenderUpdateInfo startRender(const RenderInitInfo &renderInitInfo);
		void endRender(const RenderUpdateInfo& renderUpdateInfo);
	private:
		Window window{ WIDTH, HEIGHT, "Engine_03" };
		Device device{ window };
		Renderer renderer{ window, device };

		std::unique_ptr<DescriptorPool> globalPool{};
	};
}
