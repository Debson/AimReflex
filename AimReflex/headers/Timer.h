#pragma once
#include <SDL.h>

class MDTimer
{
public:
	MDTimer();

	void start();
	void stop();
	void pause();
	void unpause();

	Uint32 getTicks();

private:
	Uint32 tStartTicks;

	Uint32 tPausedTicks;

	bool tPaused;
	bool tStarted;
};