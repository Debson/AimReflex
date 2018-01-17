#pragma once

//Points
#include "game_settings.h"

class Player 
{
public:
	Player();

	int getScore();

	int getTargetCount();

	//increase level of difficulty within score growth
	void checkScore();


	void hit();
	void miss();
	void targetMiss();
	void setMaxCombo(int combo);

	int getHits();
	int getMisses();
	int getMissedTargets();
	int getMaxCombo();
	int getTotalTargets();
	int getAiming();

	bool pHit;
	bool pMiss;

	int combo;

private:

	int pTargetCount;
	int pScore;
	int pHits;
	int pMisses;
	int pMissedTargets;
	int pMaxCombo;
	int pTotalTargets;
	int pAiming;

	bool levelEnabled[LEVEL_COUNT];

};