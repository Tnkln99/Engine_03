#include "GraphicsEngine.h"
#include "FrameInfo.h"

#include "../components/Camera.h"
#include "../components/Transform.h"
#include "../components/Model.h"
#include "../components/Light.h"

#include "../utilities/Input.h"


namespace zt::graphics
{
    GraphicsEngine::GraphicsEngine()
	{
        globalPool =
            DescriptorPool::Builder(device)
            .setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
            .build();
	}

	GraphicsEngine::~GraphicsEngine() = default;

	void GraphicsEngine::init(core::Coordinator& coordinator)
	{
        for (auto& uboBuffer : uboBuffers)
        {
            uboBuffer = std::make_unique<Buffer>(
                device,
                sizeof(GlobalUbo),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            );
            uboBuffer->map();
        }
        const auto globalSetLayout =
            DescriptorSetLayout::Builder(device)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
            .build();

        for (int i = 0; i < globalDescriptorSets.size(); i++) 
        {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            DescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

        coordinator.registerComponent<component::Camera>();
        coordinator.registerComponent<component::Transform>();
        coordinator.registerComponent<component::Model>();
        coordinator.registerComponent<component::Light>();

        modelSystem = coordinator.registerSystem<system::ModelRendererSystem>();
        {
            core::Signature signature;
            signature.set(coordinator.getComponentType<component::Model>());
            signature.set(coordinator.getComponentType<component::Transform>());
            coordinator.setSystemSignature<system::ModelRendererSystem>(signature);
        }
        modelSystem->init(device, 
            renderer.getSwapChainRenderPass(),
            globalSetLayout->getDescriptorSetLayout());

        pointLightSystem = coordinator.registerSystem<system::PointLightRenderSystem>();
        {
            core::Signature signature;
            signature.set(coordinator.getComponentType<component::Light>());
            signature.set(coordinator.getComponentType<component::Transform>());
            coordinator.setSystemSignature<system::PointLightRenderSystem>(signature);
        }
        pointLightSystem->init(device,
            renderer.getSwapChainRenderPass(),
            globalSetLayout->getDescriptorSetLayout());

        cameraControllerSystem = coordinator.registerSystem<system::CameraControllerSystem>();
        {
            core::Signature signature;
            signature.set(coordinator.getComponentType<component::Camera>());
            signature.set(coordinator.getComponentType<component::Transform>());
            coordinator.setSystemSignature<system::CameraControllerSystem>(signature);
        }
        cameraControllerSystem->init();

        utilities::Input::init(window.getGLFWwindow());
	}

    void GraphicsEngine::render(core::Coordinator& coordinator, float dt)
    {
        glfwPollEvents();

        const float aspect = renderer.getAspectRatio ();

        if (const auto commandBuffer = renderer.beginFrame())
        {
            const int frameIndex = renderer.getFrameIndex();

            renderer.beginSwapChainRenderPass(commandBuffer);

            GlobalUbo ubo{};
            const FrameInfo frameInfo
            {
                frameIndex,
                aspect,
                commandBuffer,
                globalDescriptorSets[frameIndex],
                ubo
            };

            cameraControllerSystem->update(coordinator, aspect, dt);
            modelSystem->update(coordinator, frameInfo, camera);
            pointLightSystem->update(coordinator, frameInfo);

            uboBuffers[frameIndex]->writeToBuffer(&ubo);
            uboBuffers[frameIndex]->flush();

            renderer.endSwapChainRenderPass(commandBuffer);
            renderer.endFrame();
        }

        
    }

    void GraphicsEngine::postRender()
    {
        vkDeviceWaitIdle(device.device());
        modelSystem->clean(device);
        pointLightSystem->clean(device);
    }

    bool GraphicsEngine::shouldCloseWindow()
    {
        return window.shouldClose();
    }

    std::shared_ptr<Model> GraphicsEngine::loadModel(const std::string& filePath)
    {
        return Model::createModelFromFile(device, filePath);
    }
}
