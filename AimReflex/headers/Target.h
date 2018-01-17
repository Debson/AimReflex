#pragma once
#include "MDTexture.h"
#include "game_settings.h"
#include "../Circle.h"

class Target
{
public:
	Target();

	~Target();

	void render(MDTexture *texture, SDL_Renderer &renderer);

	void handleInput(SDL_Event *e);
	// Prevent from creating target on another target

	void reset();

	bool tHit;
	bool tMiss;

	bool targetMiss;

	MDTexture getTexture();

	float getPosX();
	float getPosY();
	float getWidth();
	float getHeight();

	float oldPosX;
	float oldPosY;
	float oldWidth;
	float oldHeight;

private:
	Circle circle;
	
	SDL_Renderer *tRenderer;

	bool reverse;
	float scale;
	float tPosX;
	float tPosY;
	float tAlpha;
	bool startCircleBlend;
	
	float tWidth;
	float tHeight;
};

void checkHit();