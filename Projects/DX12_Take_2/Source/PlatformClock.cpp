#include "PCH.h"

PlatformClock::PlatformClock()
{
	m_lastTick = m_currentTick = std::chrono::high_resolution_clock::now();
	m_lifeTime = 0.0;
	Update();
}

PlatformClock::~PlatformClock()
{
}

void PlatformClock::Update()
{
	m_lastTick = m_currentTick;
	m_currentTick = std::chrono::high_resolution_clock::now();
	m_deltaTime = std::chrono::duration<double>(m_currentTick - m_lastTick).count();
	m_lifeTime += m_deltaTime;
}

double PlatformClock::GetDeltaTime()
{
	return m_deltaTime;
}

double PlatformClock::GetLifeTime()
{
	return m_lifeTime;
}
