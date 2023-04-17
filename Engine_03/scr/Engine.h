#pragma once
#include "graphics/GraphicsEngine.h"

namespace zt
{
	class Engine
	{
	public:
		Engine();
		~Engine();

		void run();
	private:
		graphics::GraphicsEngine graphicEngine{};
	};
}
