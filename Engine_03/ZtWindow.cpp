#include "ZtWindow.h"
#include <stdexcept>

namespace Zt {
	ZtWindow::ZtWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name}
	{
		inintWindow();
	}
	ZtWindow::~ZtWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}
	bool ZtWindow::shouldClose()
	{
		return glfwWindowShouldClose(window);
	}
	VkExtent2D ZtWindow::getExtent()
	{
		return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
	}
	bool ZtWindow::wasWindowResized()
	{
		return framebufferResized;
	}
	void ZtWindow::resetWindowResizedFlag()
	{
		framebufferResized = false;
	}
	void ZtWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface");
		}
	}
	void ZtWindow::frameBufferResizedCallBack(GLFWwindow* window, int width, int height)
	{
		auto ztWindow = reinterpret_cast<ZtWindow*>(glfwGetWindowUserPointer(window));
		ztWindow->framebufferResized = true;
		ztWindow->width = width;
		ztWindow->height = height;
	}
	void ZtWindow::inintWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, frameBufferResizedCallBack);
	}
}