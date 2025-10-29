#pragma once

#include "Rendering/pipeline/GameEngine_pipeline.hpp"
#include "Rendering/device/GameEngine_device.hpp"
#include "Rendering/gameObject/ge_gameObject.hpp"

#include <memory>
#include <vector>

namespace GameEngine {
    class SimpleRenderSystem {

    public:
        SimpleRenderSystem(GeDevice &device, VkRenderPass renderPass);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem&) = delete;
        SimpleRenderSystem &operator=(const SimpleRenderSystem&) = delete;
        void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GeGameObject>& gameObjects) const;

    private:
        void createPipelineLayout();
        void createPipeline(VkRenderPass render_pass);

        GeDevice& geDevice;
        std::unique_ptr<LvePipeline> gameEnginePipeline;
        VkPipelineLayout pipelineLayout;
    };
}
