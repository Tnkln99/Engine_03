#include "Engine.h"
#include "core/Coordinator.h"
#include "utilities/Timer.h"

namespace zt
{
	Engine::Engine() = default;

	Engine::~Engine() = default;

	void Engine::init(core::Coordinator& coordinator) 
	{
        coordinator.init();
        graphicEngine.init(coordinator);
        utilities::Timer::init();
	}

	void Engine::run(core::Coordinator& coordinator)
	{		
        while(!graphicEngine.shouldCloseWindow())
		{
			const float dt = utilities::Timer::getDeltaTime();
			graphicEngine.render(coordinator, dt);
		}

		graphicEngine.postRender();
	}
}
