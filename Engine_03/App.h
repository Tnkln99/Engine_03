#pragma once

#include "ZtWindow.h"
#include "ZtPipeline.h"
#include "ZtDevice.h"
#include "ZtSwapChain.h"
#include "ZtModel.h"

#include <memory>

namespace Zt
{
	class App
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		App();
		~App();

		App(const App&) = delete;
		App& operator=(const App&) = delete;

		void run();
	private:
		void loadModels();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeCommandBuffers();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);

		ZtWindow ztWindow{ WIDTH, HEIGHT, "Engine_03" };
		ZtDevice ztDevice{ ztWindow };
		std::unique_ptr<ZtSwapChain> ztSwapChain;

		std::unique_ptr<ZtPipeline> ztPipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
		std::unique_ptr<ZtModel> ztModel;
	};
}

