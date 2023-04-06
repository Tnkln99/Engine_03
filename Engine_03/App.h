#pragma once

#include "Descriptors.h"
#include "Window.h"
#include "GameObject.h"
#include "Device.h"
#include "Renderer.h"

namespace Zt
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

		Window window{ WIDTH, HEIGHT, "Engine_03" };
		Device device{ window };
		Renderer renderer{ window, device };

		std::unique_ptr<DescriptorPool> globalPool{};
		GameObject::map gameObjects;
	};
}

