#pragma once

#include "../AimReflex/headers/game_settings.h"
#include "../AimReflex/headers/Target.h"

class MDUI
{
public:
	MDUI();

	bool init(SDL_Renderer *renderer);

	void createButtonPlus(int x, int y, int h, int w);
	void createButtonMinus( int x, int y, int h, int w);
	void renderStartScreen(SDL_Color color);
	void renderPauseScreen();


	void handleInput(SDL_Event *e);
	bool handleStartScreenInput(SDL_Event *e);
	bool startCounter(int time);

	// Testing
	static Uint32 callback(Uint32 interval, void* param);

private:
	SDL_Renderer *uiRenderer;
	TTF_Font *uiFont;
	MDTimer timer;

	MDTexture textTexture;
	MDTexture buttonPlus;
	MDTexture buttonMinus;

	SDL_Color startScreenColor;
	SDL_Color textColor;

	SDL_Rect startScreenRect;
	SDL_Rect button;

	int buttonX;
	int buttonY;

	int buttonSize;

	bool timerStarted;
};