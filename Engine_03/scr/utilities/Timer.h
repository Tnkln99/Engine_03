#pragma once
#include <chrono>

namespace zt::utilities
{
	class Timer
	{
    public:
        static void init()
        {
            startTime = std::chrono::high_resolution_clock::now();
        }

        static float getElapsedTime()
        {
	        const auto currentTime = std::chrono::high_resolution_clock::now();
	        const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);
            return duration.count() / 1000.0f;
        }

        static float getDeltaTime()
        {
	        const float elapsedTime = getElapsedTime();
	        const float deltaTime = elapsedTime - lastFrameTime;
            lastFrameTime = elapsedTime;
            return deltaTime;
        }

    private:
        inline static std::chrono::time_point<std::chrono::high_resolution_clock> startTime{};
        inline static float lastFrameTime = 0.0f;
	};
}

