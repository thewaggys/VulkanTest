#include "first_app.hpp"

#include "Rendering/Shaders/simple_render_system.hpp"


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <fstream>
#include <GLFW/glfw3.h>

namespace GameEngine {
	FirstApp::FirstApp() {
		loadGameObjects();
	}
	FirstApp::~FirstApp() = default;

	void FirstApp::run() {
		SimpleRenderSystem simpleRenderSystem{geDevice, geRenderer.getSwapChainRenderPass()};
		while (!geWindow.shouldClose()) {
			glfwPollEvents();

			if (auto commandBuffer = geRenderer.beginFrame()) {

				geRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
				geRenderer.endSwapChainRenderPass(commandBuffer);
				geRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(geDevice.device());
	}

	void FirstApp::loadGameObjects()
	{
		std::vector<LveModel::Vertex> vertices {
			{{0.0f, -0.5f}, {1.0f, 1.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
		};
		auto lveModel = std::make_shared<LveModel>(geDevice, vertices);

		auto triangle = GeGameObject::createGameObject();
		triangle.model = lveModel;
		triangle.color = glm::vec3(1.0f, 0.0f, 0.0f);
		triangle.transform2d.translation.x = 0.2f;
		triangle.transform2d.scale = {2.f,0.5f};
		triangle.transform2d.rotation = .25f * glm::two_pi<float>();

		gameObjects.push_back(std::move(triangle));
	}
}