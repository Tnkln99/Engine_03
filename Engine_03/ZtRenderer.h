#pragma once

#include "ZtDevice.h"
#include "ZtWindow.h"
#include "ZtSwapChain.h"

#include <vector>
#include <cassert>

namespace Zt {
	class ZtRenderer
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;
		
		ZtRenderer(ZtWindow & window, ZtDevice& device);
		~ZtRenderer();

		ZtRenderer(const ZtRenderer&) = delete;
		ZtRenderer& operator=(const ZtRenderer&) = delete;

		[[nodiscard]] VkRenderPass getSwapChainRenderPass() const;
		[[nodiscard]] bool isFrameInProgress() const;
		[[nodiscard]] VkCommandBuffer getCurrentCommandBuffer() const;
		[[nodiscard]] int getFrameIndex() const;

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
	private:
		void createCommandBuffers();
		void recreateSwapChain();
		void freeCommandBuffers();

		ZtWindow& ztWindow;
		ZtDevice& ztDevice;

		std::unique_ptr<ZtSwapChain> ztSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex{ 0 };
		int currentFrameIndex{ 0 };
		bool isFrameStarted{ false };
	};
}
