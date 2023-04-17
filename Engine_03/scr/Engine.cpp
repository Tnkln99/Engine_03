#include "Engine.h"
#include "core/Coordinator.h"

#include <iostream>

namespace zt
{
	Engine::Engine() = default;

	Engine::~Engine() = default;

	void Engine::run()
	{		
		core::Coordinator coordinator;
		coordinator.init();

		graphicEngine.init(coordinator);
		
		while(!graphicEngine.shouldCloseWindow())
		{
			graphicEngine.render(coordinator);
		}

		graphicEngine.postRender();
	}
}
