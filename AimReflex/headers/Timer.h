#pragma once
#include <SDL.h>

class MDTimer
{
public:
	MDTimer();

	void start();
	void stop();

	Uint32 getTicks();

private:
	Uint32 mStartTicks;

	Uint32 mPausedTicks;

	bool mPaused;
	bool mStarted;
};