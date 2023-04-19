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

	void Engine::update(core::Coordinator& coordinator)
	{		
		const float dt = utilities::Timer::getDeltaTime();
		graphicEngine.render(coordinator, dt);
	}

	bool Engine::shouldCloseWindow()
	{
		return graphicEngine.shouldCloseWindow();
	}

	void Engine::terminate()
	{
		graphicEngine.postRender();
	}
}
