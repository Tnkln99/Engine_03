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

		void run(core::Coordinator& coordinator);

		graphics::GraphicsEngine graphicEngine{};
	};
}
