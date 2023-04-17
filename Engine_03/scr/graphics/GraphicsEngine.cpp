#include "GraphicsEngine.h"
#include "FrameInfo.h"

#include "../components/Camera.h"
#include "../components/Transform.h"
#include "../components/Model.h"
#include "../components/Light.h"


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

        std::shared_ptr model = Model::createModelFromFile(device, "models/flat_vase.obj");

        core::Entity vase = coordinator.createEntity();
        coordinator.addComponent<component::Transform>(
            vase,
            { { -.5f, .5f, 0.0f }, { 3.f, 1.5f, 3.f } }
        );
        coordinator.addComponent<component::Model>(
            vase,
            { model }
        );

        core::Entity vase2 = coordinator.createEntity();
        coordinator.addComponent<component::Transform>(
            vase2,
            { { .5f, .5f, 0.0f }, { 3.f, 1.5f, 3.f } }
        );
        coordinator.addComponent<component::Model>(
            vase2,
            { model }
        );

        camera = coordinator.createEntity();
        component::Transform cameraTransform{};
        cameraTransform.translation.z = -2.5f;
        coordinator.addComponent(
            camera,
            cameraTransform
        );
        component::Camera cameraComp{};
        coordinator.addComponent(
            camera,
            cameraComp
        );

        core::Entity light = coordinator.createEntity();
        component::Light compLight{};
        compLight.color = glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f };
        component::Transform lightTransform{};

        coordinator.addComponent(
            light,
            compLight
        );
        coordinator.addComponent(
            light,
            lightTransform
        );
	}

    void GraphicsEngine::render(core::Coordinator& coordinator)
    {
        glfwPollEvents();

        float aspect = renderer.getAspectRatio ();

        if (auto commandBuffer = renderer.beginFrame())
        {
            const int frameIndex = renderer.getFrameIndex();

            renderer.beginSwapChainRenderPass(commandBuffer);

            GlobalUbo ubo{};
            RenderUpdateInfo renderUpdateInfo
            {
                frameIndex,
                aspect,
                commandBuffer,
                globalDescriptorSets[frameIndex],
                ubo
            };

            cameraControllerSystem->update(coordinator, window.getGLFWwindow(), aspect, 0.01f);
            modelSystem->update(coordinator, renderUpdateInfo, camera);
            pointLightSystem->update(coordinator, renderUpdateInfo);

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
}
