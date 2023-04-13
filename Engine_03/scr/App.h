#pragma once

#include "graphics/Descriptors.h"
#include "graphics/Window.h"
#include "graphics/GameObject.h"
#include "graphics/Device.h"
#include "graphics/Renderer.h"

namespace zt
{ 
	class App 
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		App();
		~App();

		App(const App&) = delete;
		App& operator=(const App&) = delete;

		void run(); 
	private:
		void loadGameObjects();

		graphics::Window window{ WIDTH, HEIGHT, "Engine_03" };
		graphics::Device device{ window };
		graphics::Renderer renderer{ window, device };

		std::unique_ptr<graphics::DescriptorPool> globalPool{};
		graphics::GameObject::Map gameObjects;
	};
}

