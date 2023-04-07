#include "Window.h"
#include <stdexcept>

namespace zt::graphics {
	Window::Window(int w, int h, std::string name) : width{w}, height{h}, windowName{name}
	{
		initWindow();
	}
	Window::~Window()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}
	bool Window::shouldClose()
	{
		return glfwWindowShouldClose(window);
	}
	VkExtent2D Window::getExtent()
	{
		return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
	}
	bool Window::wasWindowResized()
	{
		return framebufferResized;
	}
	void Window::resetWindowResizedFlag()
	{
		framebufferResized = false;
	}

	GLFWwindow* Window::getGLFWwindow() const
	{
		return window;
	}

	void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface");
		}
	}
	void Window::frameBufferResizedCallBack(GLFWwindow* window, int width, int height)
	{
		auto ztWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		ztWindow->framebufferResized = true;
		ztWindow->width = width;
		ztWindow->height = height;
	}
	void Window::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, frameBufferResizedCallBack);
	}
}