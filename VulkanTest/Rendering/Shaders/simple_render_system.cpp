#include "simple_render_system.hpp"


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <fstream>
#include <GLFW/glfw3.h>

namespace GameEngine {

	struct SimplePushConstantData {
		glm::mat4 transform{1.f};
		alignas(16) glm::vec3 color;

	};

	SimpleRenderSystem::SimpleRenderSystem(GeDevice& device, VkRenderPass renderPass) : geDevice{device} {
		createPipelineLayout();
		createPipeline(renderPass);
	}
	SimpleRenderSystem::~SimpleRenderSystem() = default;

	void SimpleRenderSystem::createPipelineLayout()
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
		if (bool result = vkCreatePipelineLayout(geDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
			VK_SUCCESS
			) {
			throw result;
			}
		VkResult result = vkCreatePipelineLayout(geDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout);
		if (result != VK_SUCCESS) {
			throw result;
		}
	}


	void SimpleRenderSystem::createPipeline(VkRenderPass renderPass) {
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipelineLayout!");

		PipelineConfigInfo pipelineConfig{};
		LvePipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;
		gameEnginePipeline = std::make_unique<LvePipeline>(
			geDevice,
			R"(C:\Programming\Visual_Studio\Projects\VulkanTest\VulkanTest\Rendering\Shaders\simple_shader.vert.spv)",
			R"(C:\Programming\Visual_Studio\Projects\VulkanTest\VulkanTest\Rendering\Shaders\simple_shader.frag.spv)",
			pipelineConfig);
	}

	void SimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GeGameObject> &gameObjects,
	                                           const GeCamera &camera) {
		gameEnginePipeline->bind(commandBuffer);

		for (auto &obj: gameObjects) {
			obj.transform.rotation.y = glm::mod(obj.transform.rotation.y + 0.00001f, glm::two_pi<float>());
			obj.transform.rotation.x = glm::mod(obj.transform.rotation.x + 0.000005f, glm::two_pi<float>());

			SimplePushConstantData push{};
			push.color = obj.color;
			push.transform = camera.getProjection() * obj.transform.mat4();

			vkCmdPushConstants(
				commandBuffer,
				pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(SimplePushConstantData),
				&push);
			obj.model->bind(commandBuffer);
			obj.model->draw(commandBuffer);

		}
	}
}