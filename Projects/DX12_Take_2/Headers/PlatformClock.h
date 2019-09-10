#pragma once
#include <chrono>

class PlatformClock
{
public:
	PlatformClock();
	~PlatformClock();

	void Update();
	double GetDeltaTime();
	double GetLifeTime();
private:
	std::chrono::high_resolution_clock::time_point m_lastTick;
	std::chrono::high_resolution_clock::time_point m_currentTick;
	double m_deltaTime;
	double m_lifeTime;
};