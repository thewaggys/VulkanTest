#pragma once

#include "Rendering/device/GameEngine_device.hpp"


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace GameEngine {
    class LveModel {
    public:
        struct Vertex {
            glm::vec3 position;
            glm::vec3 color;

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();

            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };

        LveModel(GeDevice &device, const std::vector<Vertex> &vertices);

        ~LveModel();

        LveModel(const LveModel &) = delete;

        LveModel &operator=(const LveModel &) = delete;

        void bind(VkCommandBuffer commandbuffer) const;

        void draw(VkCommandBuffer commandbuffer) const;

    private:
        void createVertexBuffers(const std::vector<Vertex> &vertices);

        GeDevice &lveDevice;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;
    };
}