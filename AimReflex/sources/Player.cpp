#include <stdio.h>

#include "../headers/Player.h"

Player::Player()
{
	pTargetCount = START_TARGET_COUNT;

	pHit = false;
	pMiss = false;

	pScore = 0;
	pHits = 0;
	pMisses = 0;
	pMissedTargets = 0;
	pMaxCombo = 0;
	pTotalTargets = START_TARGET_COUNT;
	pAiming = 0;

	combo = 0;

	for (int i = 0; i < LEVEL_COUNT; ++i)
	{//Disable all levels on start
		levelEnabled[i] = false;
	}
}

int Player::getScore()
{
	return pScore;
}

int Player::getTargetCount()
{
	return pTargetCount;
}

void Player::checkScore()
{
	bool createNew = false;

	if (pScore > 6000 && !levelEnabled[0])
	{
		pTargetCount++;
		levelEnabled[0] = true;
	}

	if (pScore > 9000 && !levelEnabled[1])
	{
		pTargetCount++;
		levelEnabled[1] = true;
	}

	if (pScore > 12000 && !levelEnabled[2])
	{
		pTargetCount++;
		levelEnabled[2] = true;
	}
}

void Player::hit()
{
	pScore += HIT;
	pHits++;
	combo++;
	if (combo > pMaxCombo)
	{
		pMaxCombo = combo;
	}
	pTotalTargets++;
}

void Player::miss()
{
	pScore -= MISS;
	pMisses++;
	combo = 0;
	pTotalTargets++;
}

void Player::targetMiss()
{
	pScore -= TARGET_MISS;
	pMissedTargets++;
	combo = 0;
	pTotalTargets++;
}

void Player::setMaxCombo(int combo)
{
	pMaxCombo = combo;
}

int Player::getHits()
{
	return pHits;
}

int Player::getMisses()
{
	return pMisses;
}

int Player::getMissedTargets()
{
	return pMissedTargets;
}

int Player::getMaxCombo()
{
	return pMaxCombo;
}

int Player::getTotalTargets()
{
	return pTotalTargets;
}

int Player::getAiming()
{
	return pAiming;
}