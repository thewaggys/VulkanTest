#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace Lve {

	class LveWindow {

	public:
		LveWindow(int w, int h, std::string name);
		~LveWindow();

		LveWindow(const LveWindow &) = delete;
		LveWindow &operator=(const LveWindow &) = delete;

		bool shouldClose() {
			return glfwWindowShouldClose(window);
		}
		VkExtent2D getExtent() { 
			return { static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
		}

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		void initWindow(); //initiallizatios window

		const int width; //Constant height, might want to change
		const int height;

		std::string windowName; // Window Name
		GLFWwindow* window; //means i dont have to check platform

	};
}