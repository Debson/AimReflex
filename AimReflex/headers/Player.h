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

	bool pHit;
	bool pMiss;

private:

	bool levelEnabled[LEVEL_COUNT];
	bool callMissOnce;

	int pTargetCount;
	int pScore;
	int pHits;
	int pMisses;
	int pMissesTargets;
	int pMaxCombo;
};