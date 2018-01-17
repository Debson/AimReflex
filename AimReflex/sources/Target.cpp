#include <time.h>
#include <iostream>
#include <SDL.h>

#include "../headers/Target.h"

Target::Target()
{
	tPosX = (float)((rand() % (GAME_WIDTH - MAX_TARGET_WIDTH * 2)) + MAX_TARGET_WIDTH);
	tPosY = (float)((rand() % (GAME_HEIGHT - MAX_TARGET_HEIGHT * 2)) + MAX_TARGET_HEIGHT);
	scale = 0.f;
	tAlpha = 255;
	startCircleBlend = false;
	reverse = false;
	//lil
	targetMiss = false;

	tHit = false;
	tMiss = false;
}

Target::~Target()
{
	//delete this;
}

void Target::render(MDTexture *texture, SDL_Renderer &renderer)
{
	tRenderer = &renderer;
	if (scale > MAX_TARGET_SCALE)
	{
		reverse = true;
	}
	else if (scale < 0.f && !targetMiss)
	{
		//call only once
		targetMiss = true;
	}

	if (reverse)
	{
		scale -= 0.000010f + scale * 0.0005f;;
	}
	else
	{
		scale += 0.000012f + scale * 0.0009f;
	}
	float diff = texture->getHeight() * scale * 0.5f;

	tWidth = texture->getWidth() * scale;
	tHeight = texture->getHeight() * scale;


	float tempX = tPosX;
	float tempY = tPosY;
	tempX -= diff;
	tempY -= diff;
	if (startCircleBlend)
	{
		circle.draw(tRenderer, oldPosX, oldPosY, oldWidth / 2, &tAlpha);
		if (tAlpha < 0)
		{
			startCircleBlend = false;
		}
	}

	texture->render((int)tempX, (int)tempY, scale);
	/*SDL_Rect outline = { (int)tempX, (int)tempY, (int)tWidth, (int)tHeight };
	SDL_SetRenderDrawColor(&renderer, 0x00, 0xFF, 0x00, 0xFF);
	SDL_RenderDrawRect(&renderer, &outline);*/
}

void Target::handleInput(SDL_Event *e)
{
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		bool inside = true;
		tMiss = false;
		tHit = false;

		if (x < tPosX - tWidth / 2)
		{
			inside = false;
		}
		else if (x > tPosX + tWidth / 2)
		{
			inside = false;
		}
		else if (y < tPosY - tHeight / 2)
		{
			inside = false;
		}
		else if (y > tPosY + tHeight / 2)
		{
			inside = false;
		}

		if (!inside)
		{
			if (e->type == SDL_MOUSEBUTTONDOWN)
			{
				tMiss = true;
			}
		}
		else
		{
			if (e->type == SDL_MOUSEBUTTONDOWN)
			{
				tHit = true;
				startCircleBlend = true;
				reset();
			}
		}
	}
}

void Target::reset()
{
	tAlpha = 255;
	oldPosX = tPosX;
	oldPosY = tPosY;
	oldWidth = tWidth;
	oldHeight = tHeight;

	tPosX = (float)((rand() % (GAME_WIDTH - 150)) + 75);
	tPosY = (float)((rand() % (GAME_HEIGHT - 150)) + 75);
	scale = 0.f;
	reverse = false;
	targetMiss = false;
}

float Target::getPosX()
{
	return tPosX;
}

float Target::getPosY()
{
	return tPosY;
}

float Target::getWidth()
{
	return tWidth;
}

float Target::getHeight()
{
	return tHeight;
}


