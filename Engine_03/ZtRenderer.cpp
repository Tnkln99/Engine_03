#include "ZtRenderer.h"
#include <cassert>
#include <stdexcept>
#include <array>

namespace Zt 
{
	ZtRenderer::ZtRenderer(ZtWindow& window, ZtDevice& device) : ztWindow{window}, ztDevice{device}
	{
		recreateSwapChain();
		createCommandBuffers();
	}
	ZtRenderer::~ZtRenderer()
	{
		freeCommandBuffers();
	}
	VkRenderPass ZtRenderer::getSwapChainRenderPass() const
	{
		return ztSwapChain->getRenderPass();
	}
	bool ZtRenderer::isFrameInProgress() const
	{
		return isFrameStarted;
	}
	VkCommandBuffer ZtRenderer::getCurrentCommandBuffer() const
	{
		assert(isFrameStarted && "can not get command buffer when frame is not in progress");
		return commandBuffers[currentFrameIndex];
	}
	int ZtRenderer::getFrameIndex() const
	{
		assert(isFrameStarted && "can not get frame index when frame is not in progress");
		return currentFrameIndex;
	}
	VkCommandBuffer ZtRenderer::beginFrame()
	{
		assert(!isFrameStarted && "Can't call begingFrame when already in progress!");

		auto result = ztSwapChain->acquireNextImage(&currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return nullptr;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		isFrameStarted = true;

		auto commandBuffer = getCurrentCommandBuffer();
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}
		return commandBuffer;
	}
	void ZtRenderer::endFrame()
	{
		assert(isFrameStarted && "Can't call the endFrame when frame is in progress!");
		auto commandBuffer = getCurrentCommandBuffer();
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}

		auto result = ztSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
			ztWindow.wasWindowResized()) {
			ztWindow.resetWindowResizedFlag();
			recreateSwapChain();
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image!");
		}

		isFrameStarted = false;
		currentFrameIndex = (currentFrameIndex + 1) % ZtSwapChain::MAX_FRAMES_IN_FLIGHT;
	}
	void ZtRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
		assert(
			commandBuffer == getCurrentCommandBuffer() &&
			"Can't begin render pass on command buffer from a different frame");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = ztSwapChain->getRenderPass();
		renderPassInfo.framebuffer = ztSwapChain->getFrameBuffer(currentImageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = ztSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(ztSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(ztSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, ztSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}
	void ZtRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
		assert(
			commandBuffer == getCurrentCommandBuffer() &&
			"Can't end render pass on command buffer from a different frame");
		vkCmdEndRenderPass(commandBuffer);
	}
	void ZtRenderer::createCommandBuffers()
	{
		commandBuffers.resize(ZtSwapChain::MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = ztDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(ztDevice.device(), &allocInfo, commandBuffers.data()) !=
			VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers!");
		}
	}
	void ZtRenderer::recreateSwapChain()
	{
		auto extent = ztWindow.getExtent();
		while (extent.width == 0 || extent.height == 0) {
			extent = ztWindow.getExtent();
			glfwWaitEvents();
		}
		vkDeviceWaitIdle(ztDevice.device());

		if (ztSwapChain == nullptr) {
			ztSwapChain = std::make_unique<ZtSwapChain>(ztDevice, extent);
		}
		else {
			std::shared_ptr<ZtSwapChain> oldSwapChain = std::move(ztSwapChain);
			ztSwapChain = std::make_unique<ZtSwapChain>(ztDevice, extent, oldSwapChain);

			if(!oldSwapChain->compareSwapFormats(*ztSwapChain.get()))
			{
				throw std::runtime_error("Swap chain image (or depth) format has been changed!");
			}
		}
	}
	void ZtRenderer::freeCommandBuffers()
	{
		vkFreeCommandBuffers(
			ztDevice.device(),
			ztDevice.getCommandPool(),
			static_cast<uint32_t>(commandBuffers.size()),
			commandBuffers.data());
		commandBuffers.clear();
	}
}
