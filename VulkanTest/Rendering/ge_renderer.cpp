#include "ge_renderer.hpp"

#include <stdexcept>
#include <array>
#include <fstream>
#include <iostream>
#include <GLFW/glfw3.h>

namespace GameEngine {
	GeRenderer::GeRenderer(LveWindow &window, GeDevice &device) : GeWindow{window}, geDevice{device} {
		recreateSwapChain();
		createCommandBuffers();
	}
	GeRenderer::~GeRenderer() {
		vkDeviceWaitIdle(geDevice.device());
		freeCommandBuffers();
	}

	void GeRenderer::recreateSwapChain() {

		auto extent = GeWindow.getExtent();
		while (extent.width == 0 || extent.height == 0) {
			extent = GeWindow.getExtent();
			glfwWaitEvents();
		}

		if (geSwapChain == nullptr) {
			geSwapChain = std::make_unique<GameEngineSwapChain>(geDevice, extent);
		} else {
			std::shared_ptr<GameEngineSwapChain> oldSwapChain = std::move(geSwapChain);
			geSwapChain = std::make_unique<GameEngineSwapChain>(geDevice, extent, oldSwapChain);

			if (!oldSwapChain->compareSwapFormats(*geSwapChain.get())) {
				throw oldSwapChain;
			}
		}
		// piss gal
	}

	void GeRenderer::createCommandBuffers() {
		commandBuffers.resize(GameEngineSwapChain::MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = geDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t> (commandBuffers.size());

		if (bool result = vkAllocateCommandBuffers(geDevice.device(), &allocInfo, commandBuffers.data())!= VK_SUCCESS) {
			throw result;
		}
	}

	void GeRenderer::freeCommandBuffers() {
		vkFreeCommandBuffers(geDevice.device(), geDevice.getCommandPool(),
		                     static_cast<uint32_t>(commandBuffers.size()),
		                     commandBuffers.data());
		commandBuffers.clear();
	}

	VkCommandBuffer GeRenderer::beginFrame() {
		assert(!isFrameStarted && "can't call begin frame whil already in progress!");

		auto result = geSwapChain->acquireNextImage(&currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			vkDeviceWaitIdle(geDevice.device());
			recreateSwapChain();
			return nullptr;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw result;
		}

		isFrameStarted = true;

		auto commandBuffer = getCurrentCommandBuffer();
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		result = vkBeginCommandBuffer(commandBuffer, &beginInfo);
		if (result != VK_SUCCESS) {
			throw result;
		}
		return commandBuffer;
	}

	void GeRenderer::endFrame() {
		assert(isFrameStarted && "can't call end frame when frame doesn't exist");
		auto commandBuffer = getCurrentCommandBuffer();
		VkResult result = vkEndCommandBuffer(commandBuffer);
		if (result != VK_SUCCESS) {
			throw result;
		}
		result = geSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || GeWindow.wasWindowResized()) {
			GeWindow.resetWindowResizedFlag();
			recreateSwapChain();
		}
		isFrameStarted = false;
		currentFrameIndex = (currentFrameIndex + 1) % GameEngineSwapChain::MAX_FRAMES_IN_FLIGHT;
	}
	void GeRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
		assert(isFrameStarted && "can't call beginSwapChainRenderPass when frame doesn't exist");
		assert(
			commandBuffer == getCurrentCommandBuffer() &&
			"can't begin render pass on command buffer from a different frame!");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = geSwapChain->getRenderPass();
		renderPassInfo.framebuffer = geSwapChain->getFrameBuffer(currentImageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = geSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(geSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(geSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{{0,0}, geSwapChain->getSwapChainExtent()};
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}
	void GeRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) const {
		assert(isFrameStarted && "can't call endSwapChainRenderPass when frame doesn't exist");
		assert(
			commandBuffer == getCurrentCommandBuffer() &&
			"can't end render pass on command buffer from a different frame!");
		vkCmdEndRenderPass(commandBuffer);
	}

}