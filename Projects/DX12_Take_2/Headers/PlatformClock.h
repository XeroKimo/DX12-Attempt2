#pragma once
#include <chrono>

class PlatformClock
{
public:
	PlatformClock();

	void Update();
	double GetDeltaTime();
	double GetLifeTime();
private:
	std::chrono::high_resolution_clock::time_point m_lastTick = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point m_currentTick = std::chrono::high_resolution_clock::now();
	double m_deltaTime = 0.0;
	double m_lifeTime = 0.0;
};