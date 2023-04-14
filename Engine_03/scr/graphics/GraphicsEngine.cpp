#include "GraphicsEngine.h"
#include "FrameInfo.h"

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

	GraphicsEngine::~GraphicsEngine(){ }

    RenderInitInfo GraphicsEngine::init()
	{
        std::vector<std::unique_ptr<Buffer>> uboBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
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
        const auto globalSetLayout =
            DescriptorSetLayout::Builder(device)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
            .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); i++) {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            DescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

        RenderInitInfo renderInitInfo{ device ,
        	renderer.getSwapChainRenderPass(),
        	globalSetLayout->getDescriptorSetLayout(),
            globalDescriptorSets,
            uboBuffers
        };

        return renderInitInfo;
	}

    RenderUpdateInfo GraphicsEngine::startRender(const RenderInitInfo& renderInitInfo)
    {
        glfwPollEvents();

        const float aspect = renderer.getAspectRatio();

        const auto commandBuffer = renderer.beginFrame();

		int frameIndex = renderer.getFrameIndex();
        const RenderUpdateInfo renderUpdateInfo{
            frameIndex,
            aspect,
            commandBuffer,
            renderInitInfo.globalDescriptorSets[frameIndex],
            renderInitInfo.uboBuffers
        };

        renderer.beginSwapChainRenderPass(commandBuffer);

        return renderUpdateInfo;
    }

    void GraphicsEngine::endRender(const RenderUpdateInfo& renderUpdateInfo)
    {
        renderer.endSwapChainRenderPass(renderUpdateInfo.commandBuffer);
        renderer.endFrame();
    }
}
