#include <time.h>
#include <iostream>

#include "../headers/Target.h"

Target::Target()
{
	tPosX = (float)((rand() % (GAME_WIDTH - MAX_TARGET_WIDTH * 2)) + MAX_TARGET_WIDTH);
	tPosY = (float)((rand() % (GAME_HEIGHT - MAX_TARGET_HEIGHT * 2)) + MAX_TARGET_HEIGHT);
	tCurrentPosX = 0.f;
	tCurrentPosY = 0.f;
	scale = 0.f;
	tCircleAlpha = 100.f;
	tXAlpha = 100.f;
	bordersColor = { 255, 255, 255 };

	startCircleBlend = false;
	startXBlend = false;
	targetMiss = false;
	isTargetDead = false;
	reverse = false;
	tHit = false;
	tMiss = false;
	startTimer = true;
	timerPaused = false;
	timerUnpaused = false;
}

Target::~Target()
{
	xIconTexture.free();
}

void Target::render(MDTexture *texture, SDL_Renderer &renderer)
{
	tRenderer = &renderer;
	tTargetTexture = texture;
	lifeTime = timer.getTicks();
	// Scale target to MAX_TARGET_SCALE and then revert and scale it back to 0
	if (scale > MAX_TARGET_SCALE)
	{
		reverse = true;
	}
	else if (scale < 0.f && !targetMiss)
	{
		//call only once
		targetMiss = true;
	}
	else if (scale > 0.012 && startTimer)
	{
		timer.start();
		startTimer = false;
	}
	// Step for scaling
	if (reverse)
	{
		scale -= 0.0002f + scale * 0.003f;
	}
	else
	{
		scale += 0.0002f + scale * 0.003f;
	}
	// Set offset to set tPosX and tPosY exactly in center of target
	float diff = texture->getHeight() * scale * 0.5f;
	tWidth = texture->getWidth() * scale;
	tHeight = texture->getHeight() * scale;

	tCurrentPosX = tPosX;
	tCurrentPosY = tPosY;
	tCurrentPosX -= diff;
	tCurrentPosY -= diff;

	if (startCircleBlend)
	{
		circle.draw(tRenderer, oldPosX, oldPosY, oldWidth / 2, &tCircleAlpha, bordersColor);

		if (tCircleAlpha < 10.f)
		{
			startCircleBlend = false;
		}
	}
	//Render target's texture
	texture->render((int)tCurrentPosX, (int)tCurrentPosY, scale);

	// Draw target's colliders
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
				// Save target's life time on hit
				lifeTime = timer.getTicks();

				tHit = true;
				startCircleBlend = true;
			}
		}
	}
}

void Target::reset(std::vector<Target> &target, int numOfTargetHit)
{
	// Start target life time
	startTimer = true;
	// Reset
	tHit = false;
	/// Variables for drawing a circle
	tCircleAlpha = 100;
	oldPosX = tPosX;
	oldPosY = tPosY;
	oldWidth = tWidth;
	oldHeight = tHeight;

	tPosX = rand() % (GAME_WIDTH - 150) + 75;
	tPosY = rand() % (GAME_HEIGHT - 150) + 75;
	/// stay in loop and call method if newly generated position is too close to other's target position
	bool newPositionValid = true;
	while(newPositionValid)
	{
		if (getNewPos(target, numOfTargetHit))
		{
			newPositionValid = false;
		}
	}
	scale = 0.f;
	reverse = false;

	if (targetMiss)
	{
		isTargetDead = true;
	}
	if (isTargetDead)
	{
		onTargetDeath();
	}
	targetMiss = false;
}

bool Target::getNewPos(std::vector<Target> target, int numOfTargetHit)
{
	int j = 0;
	// If new poosition is close to position of other's target, call this method again
	int repeat = true;
	/* Take new position of target that was RESETED and iterate through other target's positions to check if close, if not, 
		return true, if is close, generate new position and return false	*/
	while (j < target.size())
	{
		if (numOfTargetHit != j)
		{
			if ((abs(target[numOfTargetHit].getPosY() - target[j].getPosY()) < 45.f) &&
				((abs(target[numOfTargetHit].getPosX() - target[j].getPosX())) < 45.f))
			{
				tPosX = rand() % (GAME_WIDTH - 150) + 75;
				tPosY = rand() % (GAME_HEIGHT - 150) + 75;
				//printf("%d close to each other\n", j);
				repeat = false;
			}
		}
		++j;
	}

	return repeat;
}

void Target::onTargetDeath()
{
	startXBlend = true;
}

void Target::setTargetBordersColor(SDL_Color color)
{
	bordersColor = color;
}

void Target::renderDeathX(MDTexture *texture)
{
	if (startXBlend)
	{
		float textureSizeX = 13.f;
		float textureSizeY = 13.f;
		SDL_Rect xRect = { oldPosX - textureSizeX / 2.f, oldPosY - textureSizeY / 2.f, textureSizeX, textureSizeY };

		SDL_SetTextureBlendMode(texture->getTexture(), SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(texture->getTexture(), tXAlpha);
		SDL_RenderCopy(tRenderer, texture->getTexture(), NULL, &xRect);
		tXAlpha -= 2.f;
	}

	if (tXAlpha < 0.f)
	{
		tXAlpha = 100;
		startXBlend = false;
		isTargetDead = false;
	}
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

float Target::getLifeTime()
{
	return lifeTime;
}

void Target::pauseTargetTimer()
{
	if (!timerPaused)
	{
		timer.pause();
		timerPaused = true;
	}
	tTargetTexture->render(tCurrentPosX, tCurrentPosY, scale);
}

void Target::unpauseTargetTimer()
{
	if (timerPaused)
	{
		timer.unpause();
		timerPaused = false;
	}
}

