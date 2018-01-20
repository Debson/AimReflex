#pragma once

#include "game_settings.h"
#include "Target.h"
class MDUI
{
public:
	MDUI();
	~MDUI();

	void createButtonPlus(int x, int y, int h, int w);
	void createButtonMinus( int x, int y, int h, int w);
	void renderStartScreen(SDL_Color color);
	void renderPauseScreen(SDL_Color color);
	void handleInput(SDL_Event *e);
	void resetButtonState();

	bool init(SDL_Renderer *renderer);
	bool handleStartScreenInput(SDL_Event *e);
	bool startCounter(int time, SDL_Color color);
	bool isPressed();

private:
	SDL_Renderer *uiRenderer;
	TTF_Font *uiFont;
	MDTimer timer;

	MDTexture textTexture;
	MDTexture startScreenTitle;
	MDTexture startScreenInfo[4];
	MDTexture startScreenInfoNum[4];
	MDTexture buttonPlus;
	MDTexture buttonMinus;

	SDL_Color screenColor;
	SDL_Color textColor;

	SDL_Rect startScreenRect;
	SDL_Rect button;

	int buttonX;
	int buttonY;
	int buttonSize;

	bool buttonPressed;
	bool timerStarted;
};