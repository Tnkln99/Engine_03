#pragma once
#include <GLFW/glfw3.h>
namespace zt::utilities
{
	struct Keys
	{
        int keyA = GLFW_KEY_A;
        int keyD = GLFW_KEY_D;
        int keyW = GLFW_KEY_W;
        int keyS = GLFW_KEY_S;
        int keyE = GLFW_KEY_E;
        int keyQ = GLFW_KEY_Q;
        int keyLeft = GLFW_KEY_LEFT;
        int keyRight = GLFW_KEY_RIGHT;
        int keyUp = GLFW_KEY_UP;
        int keyDown = GLFW_KEY_DOWN;
	};
    class Input {
    public:
	    inline static GLFWwindow* window = nullptr;

        static void init(GLFWwindow* window)
        {
            Input::window = window;
        }

        static bool isKeyPressed(int key)
        {
            return glfwGetKey(window, key) == GLFW_PRESS;
        }

        static bool isMouseButtonPressed(int button)
        {
            return glfwGetMouseButton(window, button) == GLFW_PRESS;
        }

        static void getMousePosition(double& xpos, double& ypos)
        {
            glfwGetCursorPos(window, &xpos, &ypos);
        }

        inline static Keys keys{};
    };
}

