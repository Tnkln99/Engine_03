#include "App.h"

#include "Camera.h"
#include "KeyboardInputController.h"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <chrono>

#include "SimpleRenderSystem.h"

namespace Zt {

    App::App() {
        loadGameObjects();
    }

    App::~App() {}

    void App::run() {
    	SimpleRenderSystem simpleRenderSystem{device, renderer.getSwapChainRenderPass()};

        Camera camera{};
    	camera.setViewTarget(glm::vec3(-1, -2, 2), glm::vec3(0, 0, 2.5f));

        auto viewObject = GameObject::createGameObject();
        KeyboardInputController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();
    	while (!window.shouldClose()) {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            frameTime = glm::min(frameTime, 4.f);
            currentTime = newTime;

            cameraController.moveInPlaneXZ(window.getGLFWwindow(), frameTime, viewObject);
            camera.setViewYXZ(viewObject.transform.translation, viewObject.transform.rotation);

            float aspect = renderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

        	if(auto commandBuffer = renderer.beginFrame())
            {
                renderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(device.device());
    }

    void App::loadGameObjects() {
        std::shared_ptr<Model> model = Model::createModelFromFile(device, "smooth_vase.obj");

        auto engineObject = GameObject::createGameObject();
        engineObject.model = model;
        engineObject.transform.translation = { .0f, .0f, 2.5f };
        engineObject.transform.scale = glm::vec3(3.f);
        gameObjects.push_back(std::move(engineObject));
    }
}