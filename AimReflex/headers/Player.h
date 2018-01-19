#pragma once
#include "../headers/Target.h"
#include <vector>

class Player 
{
public:
	Player();

	void hit();
	void miss();
	void targetMiss();
	void setMaxCombo(int combo);
	void setAiming(float aiming);
	void checkScore(std::vector<Target> *target);
	void checkTargetLifeTime(std::vector<Target> target);

	float getAiming();

	int getScore();
	int getTargetCount();
	int getHits();
	int getMisses();
	int getMissedTargets();
	int getMaxCombo();
	int getTotalTargets();

	float totalTargetsTimeAiming;
	int combo;

	bool pHit;
	bool pMiss;

private:

	int pTargetCount;
	int pScore;
	int pHits;
	int pMisses;
	int pMissedTargets;
	int pMaxCombo;
	int pTotalTargets;
	float pAiming;

	bool levelEnabled[LEVEL_COUNT];
};