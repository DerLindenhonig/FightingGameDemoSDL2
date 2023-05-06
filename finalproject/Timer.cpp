#include "Timer.h"
#include "SDL.h"

Timer* Timer::s_Instanse = nullptr; // initialize s_Instanse, because it is static

Timer::Timer()
{
}

void Timer::Tick()
{
	m_DeltaTime = (SDL_GetTicks() - m_LastTime)*(TARGET_FPS/1000.0f); // in milliseconds
	
	if (m_DeltaTime > TARGET_DELTATIME)  // // keep DeltaTime always in the same value
	{
		m_DeltaTime = TARGET_DELTATIME;
	}

	m_LastTime = SDL_GetTicks();
}
