#pragma once
#include "Descriptors.h"
#include "Renderer.h"
#include "Buffer.h"

#include "../core/Coordinator.h"


#include "../systems/ModelRendererSystem.h"
#include "../systems/PointLightRenderSystem.h"
#include "../systems/CameraControllerSystem.h"

namespace zt::graphics
{
	class GraphicsEngine
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		GraphicsEngine();
		~GraphicsEngine();

		GraphicsEngine(const GraphicsEngine&) = delete;
		GraphicsEngine& operator=(const GraphicsEngine&) = delete;

		void init(core::Coordinator& coordinator);
		void render(core::Coordinator& coordinator);
		void postRender();

		bool shouldCloseWindow();
	private:
		Window window{ WIDTH, HEIGHT, "Engine_03" };
		Device device{ window };
		Renderer renderer{ window, device };

		std::unique_ptr<DescriptorPool> globalPool{};
		std::vector<VkDescriptorSet> globalDescriptorSets{ SwapChain::MAX_FRAMES_IN_FLIGHT };
		std::vector<std::unique_ptr<Buffer>> uboBuffers{ SwapChain::MAX_FRAMES_IN_FLIGHT };

		std::shared_ptr<system::ModelRendererSystem> modelSystem;
		std::shared_ptr<system::PointLightRenderSystem> pointLightSystem;
		std::shared_ptr<system::CameraControllerSystem> cameraControllerSystem;
		core::Entity camera{};
	};
}
