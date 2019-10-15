#include "PCH.h"

using namespace std::chrono;

PlatformClock::PlatformClock()
{
    m_initialTick = m_previousTick = m_currentTick = high_resolution_clock::now();
    Tick();

}

void PlatformClock::Tick()
{
	m_previousTick = m_currentTick;
	m_currentTick = high_resolution_clock::now();
    m_deltaTick = m_currentTick - m_previousTick;
}

