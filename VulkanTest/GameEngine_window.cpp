#include "GameEngine_window.hpp"

#include <stdexcept>

namespace GameEngine {
	LveWindow::LveWindow(int w, int h, std::string const & name) : width{ w }, height{ h }, windowName{ name } {
		initWindow(); //Initialise window
	}

	LveWindow::~LveWindow() //handles closing the window
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void LveWindow::initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //turn off vulkan api for this
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	}


	void LveWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
		VkResult result = glfwCreateWindowSurface(instance, window, nullptr, surface);
		if (result != VK_SUCCESS) {
			throw result;
		}


	}
	void LveWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto lveWindow = static_cast<LveWindow*>(glfwGetWindowUserPointer(window));
		lveWindow->framebufferResized = true;
		lveWindow->width = width;
		lveWindow->height = height;
	}
}