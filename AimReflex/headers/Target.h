#pragma once
#include "MDTexture.h"
#include "game_settings.h"

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


private:
	
	bool reverse;
	float scale;
	float tPosX;
	float tPosY;
	
	float tWidth;
	float tHeight;
};

void checkHit();