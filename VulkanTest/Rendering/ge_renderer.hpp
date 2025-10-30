#pragma once

#include "Rendering/device/window/GameEngine_window.hpp"
#include "Rendering/swapchain/GameEngine_swap_chain.hpp"
#include "Rendering/device/GameEngine_device.hpp"

#include <memory>
#include <vector>
#include <cassert>

namespace GameEngine {
    class GeRenderer {

    public:

        GeRenderer(LveWindow &window, GeDevice &device);
        ~GeRenderer();

        GeRenderer(const GeRenderer&) = delete;
        GeRenderer &operator=(const GeRenderer&) = delete;

        VkRenderPass getSwapChainRenderPass() const { return geSwapChain->getRenderPass(); };
        float getAspectRatio() const { return geSwapChain->extentAspectRatio(); };
        bool isFrameInProgress() const {return isFrameStarted;}

        VkCommandBuffer getCurrentCommandBuffer() const {
            assert(isFrameStarted && "cannot get command buffer when frame is not in progress");
            return commandBuffers[currentFrameIndex];
        };

        int getFrameIndex() const {
            assert(isFrameStarted && "cannot get frame index when frame is not in progress");
            return currentFrameIndex;
        }

        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer) const;
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer) const;

    private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();

        LveWindow &GeWindow;
        GeDevice &geDevice;
        std::unique_ptr<GameEngineSwapChain> geSwapChain;
        std::vector<VkCommandBuffer> commandBuffers;
        uint32_t currentImageIndex;
        int currentFrameIndex{0};
        bool isFrameStarted{false};
    };
}
