#define _USE_MATH_DEFINES

#include <iostream>
#include "../headers/Circle.h"

void Circle::draw(SDL_Renderer *renderer, float centerX, float centerY, float radiusX, float *alpha, SDL_Color color)
{
	mRenderer = renderer;
	mRadius = radiusX;

	const int precision = 60;
	float alfa = 0;

	float x = radiusX * cos(alfa);
	float y = radiusX * sin(alfa);
	xPos = x;
	yPos = y;
	float step = M_PI_2 / (float)precision;
	for (alfa = step; alfa <= M_PI_2; alfa += step)
	{
		xPos = radiusX * cosf(alfa) + 0.5;
		yPos = radiusX * sinf(alfa) + 0.5;

		if ((x != xPos) || (y != yPos))
		{
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, *alpha);

			SDL_RenderDrawLine(renderer, centerX + x, centerY - y, centerX + xPos, centerY - yPos);
			SDL_RenderDrawLine(renderer, centerX - x, centerY - y, centerX - xPos, centerY - yPos);
			SDL_RenderDrawLine(renderer, centerX - x, centerY + y, centerX - xPos, centerY + yPos);
			SDL_RenderDrawLine(renderer, centerX + x, centerY + y, centerX + xPos, centerY + yPos);
			//TODO use easing function
			*alpha -= 0.05f;
		}
		x = xPos;
		y = yPos;
	}
	if (x != 0)
	{
		x = 0;
		SDL_RenderDrawLine(renderer, centerX + x, centerY - y, centerX + xPos, centerY - yPos);
		SDL_RenderDrawLine(renderer, centerX - x, centerY - y, centerX - xPos, centerY - yPos);
		SDL_RenderDrawLine(renderer, centerX - x, centerY + y, centerX - xPos, centerY + yPos);
		SDL_RenderDrawLine(renderer, centerX + x, centerY + y, centerX + xPos, centerY + yPos);
	}
}
