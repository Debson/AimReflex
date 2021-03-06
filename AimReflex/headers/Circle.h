#pragma once
#include <SDL.h>
#include "game_settings.h"

class Circle
{
public:
	// Draws circle and decreases transparency with time
	void draw(SDL_Renderer *renderer, float x, float y, float radiusX, float *alpha, SDL_Color color, float deltaTime);

private:
	SDL_Renderer *mRenderer;

	float xPos;
	float yPos;
	float mRadius;
	float mAlpha;
};