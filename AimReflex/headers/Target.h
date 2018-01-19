#pragma once

#include <vector>
#include "MDTexture.h"
#include "game_settings.h"
#include "../headers/Circle.h"
#include "../headers/Timer.h"

class Target
{
public:
	Target();

	~Target();

	void render(MDTexture *texture, SDL_Renderer &renderer);

	void handleInput(SDL_Event *e);

	// Prevent from creating target on another target

	void renderDeathX(MDTexture *texture);
	void reset(std::vector<Target> &target, int numOfTargetHit);
	void onTargetDeath();

	bool tHit;
	bool tMiss;

	bool targetMiss;

	bool getNewPos(std::vector<Target> target, int numOfTargetHit);


	float getPosX();
	float getPosY();
	float getWidth();
	float getHeight();
	float getLifeTime();
	void pauseTargetTimer();
	void unpauseTargetTimer();


	float oldPosX;
	float oldPosY;
	float oldWidth;
	float oldHeight;

private:
	Circle circle;
	MDTimer timer;
	MDTexture xIconTexture;

	SDL_Renderer *tRenderer;
	SDL_Texture *tTexture;

	bool timerPaused;
	bool timerUnpaused;

	float lifeTime;

	bool reverse;
	float scale;
	float tPosX;
	float tPosY;

	float tCircleAlpha;
	float tXAlpha;

	bool isTargetDead;

	bool startTimer;


	bool startCircleBlend;
	bool startXBlend;
	
	float tWidth;
	float tHeight;
};
