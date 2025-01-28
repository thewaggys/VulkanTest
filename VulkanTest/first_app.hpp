#pragma once

#include "Lve_window.hpp"
#include "Lve_pipeline.hpp"
#include "Lve_swap_chain.hpp"
#include "Lve_device.hpp"

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
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();

		LveWindow LveWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
		RenderDevice LveDevice{LveWindow};
		LveSwapChain lveSwapChain{ LveDevice, LveWindow.getExtent()};
		std::unique_ptr<LvePipeline> lvePipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
	};
}