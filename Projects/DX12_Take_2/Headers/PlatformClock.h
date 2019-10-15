#pragma once
#include <chrono>

class PlatformClock
{
public:
	PlatformClock();

	void Tick();
    template<class ChronoType = std::chrono::nanoseconds>
    ChronoType GetChronoDeltaTime() { return std::chrono::duration_cast<ChronoType>(m_deltaTick); }
    template<class ChronoType = std::chrono::nanoseconds>
    ChronoType GetChronoLifeTime() { return std::chrono::duration_cast<ChronoType>(std::chrono::high_resolution_clock::now() - m_initialTick); }

    template <typename Type>
    Type GetDeltaTime();
    template <typename Type>
    Type GetLifeTime();
private:
    std::chrono::high_resolution_clock::time_point m_initialTick = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point m_previousTick = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point m_currentTick = std::chrono::high_resolution_clock::now();
    std::chrono::nanoseconds m_deltaTick;
};

template <typename Type>
inline Type PlatformClock::GetDeltaTime()
{
    return std::chrono::duration<Type>(m_deltaTick).count();
}

template <typename Type>
inline Type PlatformClock::GetLifeTime()
{
    return std::chrono::duration<Type>(std::chrono::high_resolution_clock::now() - m_initialTick).count();
}
