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
		bool wasWindowResized() {return framebufferResized;}
		void resetWindowResizedFlag() { framebufferResized = false; }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
		void initWindow(); //initiallizatios window

		int width; //Constant height, might want to change
		int height;
		bool framebufferResized = false;

		std::string windowName; // Window Name
		GLFWwindow* window; //means i dont have to check platform

	};
}