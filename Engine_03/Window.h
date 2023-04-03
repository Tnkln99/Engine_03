#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>;

namespace Zt
{
	class Window
	{
	public:
		Window(int w, int h, std::string name);
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		bool shouldClose();
		VkExtent2D getExtent();

		bool wasWindowResized();
		void resetWindowResizedFlag();
		GLFWwindow* getGLFWwindow() const;

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
	private:
		static void frameBufferResizedCallBack(GLFWwindow* window, int width, int height);

		void initWindow();

		int width;
		int height;
		bool framebufferResized = false;

		std::string windowName;
		GLFWwindow* window;
	};
}

