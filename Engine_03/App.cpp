#include "App.h"
// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "SimpleRenderSystem.h"

namespace Zt {

    App::App() {
        loadGameObjects();
    }

    App::~App() {}

    void App::run() {
        SimpleRenderSystem simpleRenderSystem{ztDevice, ztRenderer.getSwapChainRenderPass()};
        while (!ztWindow.shouldClose()) {
            glfwPollEvents();
            if(auto commandBuffer = ztRenderer.beginFrame())
            {
                ztRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
                ztRenderer.endSwapChainRenderPass(commandBuffer);
                ztRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(ztDevice.device());
    }

    void App::loadGameObjects() {
        std::vector<ZtModel::Vertex> vertices{
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        };
        auto ztModel = std::make_shared<ZtModel>(ztDevice, vertices);
        auto triangle = ZtGameObject::createGameObject();
        triangle.model = ztModel;
        triangle.color = { 0.1f, .8f, .1f };
        triangle.transform2d.translation.x = .2f;
        //triangle.transform2d.scale = { 2.f, .5f };
        //triangle.transform2d.rotation = .25f * glm::two_pi<float>();

        gameObjects.push_back(std::move(triangle));
    }
}