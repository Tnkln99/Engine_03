#pragma once

#include "Descriptors.h"
#include "Window.h"
#include "GameObject.h"
#include "Device.h"
#include "Renderer.h"

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
		graphics::GameObject::map gameObjects;
	};
}

