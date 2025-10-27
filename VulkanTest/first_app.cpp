#include "first_app.hpp"


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <stdexcept>
#include <array>
#include <iostream>
#include <fstream>
#include <GLFW/glfw3.h>

namespace GameEngine {

	struct SimplePushConstantData {
		glm::vec2 offset;
		alignas(16) glm::vec3 color;

	};

	FirstApp::FirstApp() {
		loadModels();
		createPipelineLayout();
		recreateSwapChain();
		createCommandBuffers();
	}
	FirstApp::~FirstApp() {
		vkDestroyPipelineLayout(GameEngineDevice.device(), pipelineLayout, nullptr);
	}

	void FirstApp::run() {
		while (!LveWindow.shouldClose()) {
			glfwPollEvents();
			drawFrame();
		}

		vkDeviceWaitIdle(GameEngineDevice.device());
	}

	void FirstApp::loadModels()
	{
		std::vector<LveModel::Vertex> vertices {
			{{0.0f, -0.5f}, {1.0f, 1.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
		};

		for (const auto& vertex : vertices) {
			std::cout << "Color: " << vertex.color.x << ", " << vertex.color.y << ", " << vertex.color.z << std::endl;
		}

		gameEngineModel = std::make_unique<LveModel>(GameEngineDevice, vertices);
	}

	void FirstApp::createPipelineLayout()
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);


		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
		if (vkCreatePipelineLayout(GameEngineDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
			VK_SUCCESS
			) {
			throw std::runtime_error("failed to create pipeline layout!");
		}
	}


	void FirstApp::createPipeline() {
		assert(gameEngineSwapChain != nullptr && "Cannot create pipeline before swapchain!");
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipelineLayout!");

		PipelineConfigInfo pipelineConfig{};
		LvePipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = gameEngineSwapChain->getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		gameEnginePipeline = std::make_unique<LvePipeline>(
			GameEngineDevice,
			R"(C:\Programming\Visual_Studio\Projects\VulkanTest\VulkanTest\Shaders\simple_shader.vert.spv)",
			R"(C:\Programming\Visual_Studio\Projects\VulkanTest\VulkanTest\Shaders\simple_shader.frag.spv)",
			pipelineConfig);
	}

	void FirstApp::recreateSwapChain() {
		auto extent = LveWindow.getExtent();
		while (extent.width == 0 || extent.height == 0) {
			extent = LveWindow.getExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(GameEngineDevice.device());

		if (gameEngineSwapChain == nullptr) {
			gameEngineSwapChain = std::make_unique<GameEngineSwapChain>(GameEngineDevice, extent);
		} else {
			gameEngineSwapChain = std::make_unique<GameEngineSwapChain>(GameEngineDevice, extent, std::move(gameEngineSwapChain));
			if (gameEngineSwapChain->imageCount() != commandBuffers.size()) {
				freeCommandBuffers();
				createCommandBuffers();
			}
		}
		createPipeline();
	}

	void FirstApp::createCommandBuffers() {

		commandBuffers.resize(gameEngineSwapChain->imageCount());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = GameEngineDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t> (commandBuffers.size());

		if (vkAllocateCommandBuffers(GameEngineDevice.device(), &allocInfo, commandBuffers.data()) !=
			VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers!");
		}
	}

	void FirstApp::freeCommandBuffers() {
		vkFreeCommandBuffers(GameEngineDevice.device(), GameEngineDevice.getCommandPool(),
		                     static_cast<uint32_t>(commandBuffers.size()),
		                     commandBuffers.data());
		commandBuffers.clear();
	}

	void FirstApp::registerModels(std::unique_ptr<LveModel>& model, int imageIndex) {
       model->bind(commandBuffers[imageIndex]);
       model->draw(commandBuffers[imageIndex]);
    }

	void FirstApp::recordCommandBuffer(int imageIndex) {
		static int frame = 30;
		frame = (frame + 1) % 100;
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = gameEngineSwapChain->getRenderPass();
		renderPassInfo.framebuffer = gameEngineSwapChain->getFrameBuffer(imageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = gameEngineSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(gameEngineSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(gameEngineSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{{0,0}, gameEngineSwapChain->getSwapChainExtent()};
		vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
		vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

		gameEnginePipeline->bind(commandBuffers[imageIndex]);
		gameEngineModel->bind(commandBuffers[imageIndex]);

		for (float j = 0.0f;j < 4; j++) {
			SimplePushConstantData push{};
			push.offset = {0.0f, -0.4f + j * 0.25f};
			push.color = {0.0, 0.0, 0.2f + 0.2f * j};

			vkCmdPushConstants(
				commandBuffers[imageIndex],
				pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(SimplePushConstantData),
				&push);
			gameEngineModel->draw(commandBuffers[imageIndex]);
		}

		vkCmdEndRenderPass(commandBuffers[imageIndex]);
		if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}

	void FirstApp::drawFrame() {
		uint32_t imageIndex;
		auto result = gameEngineSwapChain->acquireNextImage(&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		recordCommandBuffer(imageIndex);
		result = gameEngineSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || LveWindow.wasWindowResized()) {
			LveWindow.resetWindowResizedFlag();
			recreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image!");
		}
	
	}

}