#pragma once
#include <SDL.h>

class Circle
{
public:
	Circle();

	// Draw elipse, need to provide 2 radius.
	void draw(SDL_Renderer *renderer, float x, float y, float radiusX, float radiusY);
	// Draws circle and decreases transparency with time
	void draw(SDL_Renderer *renderer, float x, float y, float radiusX, float *alpha);
	// Draw circle
	void draw(SDL_Renderer *renderer, float x, float y, float radiusX);

private:

	SDL_Renderer *mRenderer;

	float xPos;
	float yPos;
	float mRadius;
	float mAlpha;
};