#pragma once

#include <vector>
#include "MDTexture.h"
#include "game_settings.h"
#include "Circle.h"
#include "Timer.h"

class Target
{
public:
	Target();

	~Target();

	void render(MDTexture *texture, SDL_Renderer &renderer);
	void handleInput(SDL_Event *e);
	void renderDeathX(MDTexture *texture);
	void reset(std::vector<Target> &target, int numOfTargetHit);
	void onTargetDeath();
	void setTargetBordersColor(SDL_Color);
	void pauseTargetTimer();
	void unpauseTargetTimer();

	float getPosX();
	float getPosY();
	float getWidth();
	float getHeight();
	float getLifeTime();

	bool getNewPos(std::vector<Target> target, int numOfTargetHit);

	bool tHit;
	bool tMiss;
	bool targetMiss;

private:
	MDTimer timer;
	Circle circle;

	MDTexture *tTargetTexture;
	MDTexture xIconTexture;
	SDL_Renderer *tRenderer;
	SDL_Texture *tTexture;
	SDL_Color bordersColor;

	float lifeTime;
	float scale;
	float tPosX;
	float tPosY;
	float oldPosX;
	float oldPosY;
	float oldWidth;
	float oldHeight;
	float tCurrentPosX;
	float tCurrentPosY;
	float tCircleAlpha;
	float tXAlpha;
	float tWidth;
	float tHeight;

	bool reverse;
	bool isTargetDead;
	bool startTimer;
	bool startCircleBlend;
	bool startXBlend;
	bool timerPaused;
	bool timerUnpaused;
};
