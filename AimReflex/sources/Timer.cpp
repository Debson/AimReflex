#include "../headers/Timer.h"

MDTimer::MDTimer()
{
	tStartTicks = 0;
	tPausedTicks = 0;

	tPaused = false;
	tStarted = false;
}

void MDTimer::start()
{
	tStarted = true;

	tPaused = false;

	tStartTicks = SDL_GetTicks();
	tPausedTicks = 0;
}

void MDTimer::stop()
{
	tStarted = false;

	tPaused = false;

	tStartTicks = 0;
	tPausedTicks = 0;
}

void MDTimer::pause()
{
	if (tStarted && !tPaused)
	{
		tPaused = true;

		tPausedTicks = SDL_GetTicks() - tStartTicks;
		tStartTicks = 0;
	}
}

void MDTimer::unpause()
{
	if (tStarted && tPaused)
	{
		tPaused = false;

		tStartTicks = SDL_GetTicks() - tPausedTicks;

		tPausedTicks = 0;
	}
}

Uint32 MDTimer::getTicks()
{
	Uint32 time = 0;

	if (tStarted)
	{
		if (tPaused)
		{
			time = tPausedTicks;
		}
		else
		{
			time = SDL_GetTicks() - tStartTicks;
		}
	}

	return time;
}