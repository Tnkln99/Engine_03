#include "App.h"

#include "graphics/Camera.h"
#include "graphics/KeyboardInputController.h"
#include "graphics/Buffer.h"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <chrono>

#include "graphics/SimpleRenderSystem.h"
#include "graphics/PointLightSystem.h"

namespace zt {

    struct GlobalUbo {
        glm::mat4 projectionMatrix{ 1.f };
        glm::mat4 viewMatrix{ 1.f };
        glm::vec4 ambientLightColor{ 1.0f, 1.0f, 1.0f, .02f };
        glm::vec3 lightPosition{ 0.0f,-2.0f,0.0f };
        alignas(16) glm::vec4 lightColor{ 1.0f, 1.0f, 1.0f, 1.0f };
    };

    App::App() {
        globalPool =
            graphics::DescriptorPool::Builder(device)
            .setMaxSets(graphics::SwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, graphics::SwapChain::MAX_FRAMES_IN_FLIGHT)
            .build();
        loadGameObjects();
    }

    App::~App() {}

    void App::run() {
        std::vector<std::unique_ptr<graphics::Buffer>> uboBuffers(graphics::SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (auto& uboBuffer : uboBuffers)
        {
	        uboBuffer = std::make_unique<graphics::Buffer>(
                device,
                sizeof(GlobalUbo),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            );
	        uboBuffer->map();
        }
        auto globalSetLayout =
            graphics::DescriptorSetLayout::Builder(device)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
            .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(graphics::SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); i++) {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            graphics::DescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

    	graphics::SimpleRenderSystem simpleRenderSystem{device,
    		renderer.getSwapChainRenderPass(),
			globalSetLayout->getDescriptorSetLayout()
    	};

        graphics::PointLightSystem pointLightSystem{ device,
            renderer.getSwapChainRenderPass(),
            globalSetLayout->getDescriptorSetLayout()
        };

        graphics::Camera camera{};
    	camera.setViewTarget(glm::vec3(-1, -2, 2), glm::vec3(0, 0, 2.5f));

        auto viewObject = graphics::GameObject::createGameObject();
        viewObject.transform.translation.z = -2.5f;
        graphics::KeyboardInputController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();
    	while (!window.shouldClose()) 
        {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            frameTime = glm::min(frameTime, 4.f);
            currentTime = newTime;

            cameraController.moveInPlaneXZ(window.getGLFWwindow(), frameTime, viewObject);
            camera.setViewYXZ(viewObject.transform.translation, viewObject.transform.rotation);

            float aspect = renderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

        	if(auto commandBuffer = renderer.beginFrame())
            {
                int frameIndex = renderer.getFrameIndex();
                graphics::FrameInfo frameInfo{
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera,
                    globalDescriptorSets[frameIndex],
                	gameObjects
                };

        		// render 
                renderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(frameInfo);
                pointLightSystem.render(frameInfo);
                GlobalUbo ubo{};

                ubo.lightColor = glm::vec4{ 1.0f,0.0f,1.0f,1.0f };
                ubo.projectionMatrix = camera.getProjection();
                ubo.viewMatrix = camera.getView();
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                

                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();        		
            }
        }

        vkDeviceWaitIdle(device.device());
    }

    void App::loadGameObjects() {
        std::shared_ptr model = graphics::Model::createModelFromFile(device, "models/flat_vase.obj");
        auto flatVase = graphics::GameObject::createGameObject();
        flatVase.model = model;
        flatVase.transform.translation = { -.5f, .5f, 0.0f };
        flatVase.transform.scale = { 3.f, 1.5f, 3.f };
        gameObjects.emplace(flatVase.getId(), std::move(flatVase));

        model = graphics::Model::createModelFromFile(device, "models/smooth_vase.obj");
        auto smoothVase = graphics::GameObject::createGameObject();
        smoothVase.model = model;
        smoothVase.transform.translation = { .5f, .5f, 0.0f };
        smoothVase.transform.scale = { 3.f, 1.5f, 3.f };
        gameObjects.emplace(smoothVase.getId(), std::move(smoothVase));

        model = graphics::Model::createModelFromFile(device, "models/quad.obj");
        auto floor = graphics::GameObject::createGameObject();
        floor.model = model;
        floor.transform.translation = { 0.0f, .5f, 0.0f };
        floor.transform.scale = { 3.f, 1.f, 3.f };
        gameObjects.emplace(floor.getId(), std::move(floor));
    }
}