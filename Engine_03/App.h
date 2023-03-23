#pragma once

#include "ZtWindow.h"
#include "ZtGameObject.h"
#include "ZtDevice.h"
#include "ZtRenderer.h"

#include <memory>

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

		ZtWindow ztWindow{ WIDTH, HEIGHT, "Engine_03" };
		ZtDevice ztDevice{ ztWindow };
		ZtRenderer ztRenderer{ ztWindow, ztDevice };

		std::vector<ZtGameObject> gameObjects;
	};
}

