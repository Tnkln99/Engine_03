#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>;

namespace Zt
{
	class ZtWindow
	{
	public:
		ZtWindow(int w, int h, std::string name);
		~ZtWindow();

		ZtWindow(const ZtWindow&) = delete;
		ZtWindow& operator=(const ZtWindow&) = delete;

		bool shouldClose();
		VkExtent2D getExtent();

		bool wasWindowResized();
		void resetWindowResizedFlag();

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
	private:
		static void frameBufferResizedCallBack(GLFWwindow* window, int width, int height);

		void inintWindow();

		int width;
		int height;
		bool framebufferResized = false;

		std::string windowName;
		GLFWwindow* window;
	};
}

