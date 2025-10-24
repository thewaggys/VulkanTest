#pragma once

#include "GameEngine_window.hpp"
#include "GameEngine_pipeline.hpp"
#include "GameEngine_swap_chain.hpp"
#include "GameEngine_device.hpp"
#include "GameEngine_model.hpp"

#include <memory>
#include <vector>

namespace Lve {
	class FirstApp {

	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp &operator=(const FirstApp&) = delete;

		void run();
	private:
		void loadModels();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);

		LveWindow LveWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
		RenderDevice LveDevice{LveWindow};
		std::unique_ptr<GameEngineSwapChain> lveSwapChain;
		std::unique_ptr<LvePipeline> lvePipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
		std::unique_ptr<LveModel> lveModel;
	};
}