#pragma once

#include "Device.h"
#include "Window.h"
#include "SwapChain.h"

#include <vector>

namespace zt::graphics {
	class Renderer
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;
		
		Renderer(Window & window, Device& device);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		[[nodiscard]] VkRenderPass getSwapChainRenderPass() const;
		[[nodiscard]] float getAspectRatio() const;
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

		Window& window;
		Device& device;

		std::unique_ptr<SwapChain> swapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex{ 0 };
		int currentFrameIndex{ 0 };
		bool isFrameStarted{ false };
	};
}
