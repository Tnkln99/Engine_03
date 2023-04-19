#pragma once
#include "graphics/GraphicsEngine.h"

namespace zt
{
	class Engine
	{
	public:
		Engine();
		~Engine();

		void init(core::Coordinator& coordinator);

		void update(core::Coordinator& coordinator);

		bool shouldCloseWindow();

		void terminate();

		graphics::GraphicsEngine graphicEngine{};
	};
}
