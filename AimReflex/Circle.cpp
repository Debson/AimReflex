#define _USE_MATH_DEFINES

#include "Circle.h"

Circle::Circle()
{
	mAlpha = 255;
}

void Circle::draw(SDL_Renderer *renderer, float centerX, float centerY, float radiusX, float *alpha)
{
	mRenderer = renderer;
	mRadius = radiusX;

	const int precision = 27;
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
			SDL_SetRenderDrawColor(renderer, 0, *alpha, 0, 0);
			SDL_RenderDrawLine(renderer, centerX + x, centerY - y, centerX + xPos, centerY - yPos);
			SDL_RenderDrawLine(renderer, centerX - x, centerY - y, centerX - xPos, centerY - yPos);
			SDL_RenderDrawLine(renderer, centerX - x, centerY + y, centerX - xPos, centerY + yPos);
			SDL_RenderDrawLine(renderer, centerX + x, centerY + y, centerX + xPos, centerY + yPos);
			*alpha -= 0.025f;
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

void Circle::draw(SDL_Renderer *renderer, float centerX, float centerY, float radiusX)
{
	mRenderer = renderer;
	mRadius = radiusX;

	const int precision = 27;
	float alfa = 0;

	int x = radiusX * cos(alfa);
	int y = radiusX * sin(alfa);
	int xPos = x;
	int yPos = y;

	float step = M_PI_2 / (float)precision;
	for (alfa = step; alfa <= M_PI_2; alfa += step)
	{
		xPos = radiusX * cosf(alfa) + 0.5;
		yPos = radiusX * sinf(alfa) + 0.5;

		if ((x != xPos) || (y != yPos))
		{
			SDL_RenderDrawLine(renderer, centerX + x, centerY - y, centerX + xPos, centerY - yPos);
			SDL_RenderDrawLine(renderer, centerX - x, centerY - y, centerX - xPos, centerY - yPos);
			SDL_RenderDrawLine(renderer, centerX - x, centerY + y, centerX - xPos, centerY + yPos);
			SDL_RenderDrawLine(renderer, centerX + x, centerY + y, centerX + xPos, centerY + yPos);
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

void Circle::draw(SDL_Renderer *renderer, float centerX, float centerY, float radiusX, float radiusY)
{
	mRenderer = renderer;
	mRadius = radiusX;

	const int precision = 27;
	float alfa = 0;

	int x = radiusX * cos(alfa);
	int y = radiusX * sin(alfa);
	int xPos = x;
	int yPos = y;

	float step = M_PI_2 / (float)precision;
	for (alfa = step; alfa <= M_PI_2; alfa += step)
	{
		xPos = radiusX * cosf(alfa) + 0.5;
		yPos = radiusY * sinf(alfa) + 0.5;

		if ((x != xPos) || (y != yPos))
		{
			SDL_RenderDrawLine(renderer, centerX + x, centerY - y, centerX + xPos, centerY - yPos);
			SDL_RenderDrawLine(renderer, centerX - x, centerY - y, centerX - xPos, centerY - yPos);
			SDL_RenderDrawLine(renderer, centerX - x, centerY + y, centerX - xPos, centerY + yPos);
			SDL_RenderDrawLine(renderer, centerX + x, centerY + y, centerX + xPos, centerY + yPos);
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

