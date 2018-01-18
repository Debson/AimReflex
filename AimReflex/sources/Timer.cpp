#include "Timer.h"

MDTimer::MDTimer()
{
	mStartTicks = 0;
	mPausedTicks = 0;

	mPaused = false;
	mStarted = false;
}

void MDTimer::start()
{
	mStarted = true;

	mPaused = false;

	mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void MDTimer::stop()
{
	mStarted = false;

	mPaused = false;

	mStartTicks = 0;
	mPausedTicks = 0;
}

Uint32 MDTimer::getTicks()
{
	Uint32 time = 0;

	if (mStarted)
	{
		if (mPaused)
		{
			time = mPausedTicks;
		}
		else
		{
			time = SDL_GetTicks() - mStartTicks;
		}
	}

	return time;
}