#include "first_app.hpp"
#include "Rendering/ge_camera.hpp"

#include "Rendering/Shaders/simple_render_system.hpp"


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <fstream>
#include <GLFW/glfw3.h>


namespace GameEngine {
	FirstApp::FirstApp() {
		loadGameObjects();
	}
	FirstApp::~FirstApp() = default;

	void FirstApp::run() {
		SimpleRenderSystem simpleRenderSystem{geDevice, geRenderer.getSwapChainRenderPass()};
		GeCamera camera{};

		while (!geWindow.shouldClose()) {
			glfwPollEvents();

			float aspectRatio = geRenderer.getAspectRatio();
			camera.setPerspectiveProjection(glm::radians(50.f), aspectRatio, 0.1f, 10.f);

			if (auto commandBuffer = geRenderer.beginFrame()) {

				geRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
				geRenderer.endSwapChainRenderPass(commandBuffer);
				geRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(geDevice.device());
	}

	std::unique_ptr<LveModel> createCubeModel(GeDevice &device, glm::vec3 offset) {
		std::vector<LveModel::Vertex> vertices{

			// left face (white)
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

			// right face (yellow)
			{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},

			// top face (orange, remember y axis points down)
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

			// bottom face (red)
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},

			// nose face (blue)
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

			// tail face (green)
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

		};
		for (auto &v: vertices) {
			v.position += offset;
		}
		return std::make_unique<LveModel>(device, vertices);
	}

	void FirstApp::loadGameObjects()
	{
		std::shared_ptr<LveModel> geModel = createCubeModel(geDevice, glm::vec3{0.f});
		auto cube = GeGameObject::createGameObject();
		cube.model = geModel;
		cube.transform.translation = {0.f, 0.f, 2.5f};
		cube.transform.scale = {.5f, 0.5f, 0.5f};
		gameObjects.push_back(std::move(cube));
	}
}
